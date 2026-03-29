"""
Visualisation de la simulation
Lit : positions.csv, velocities.csv, energies.csv, rdf.csv
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.gridspec import GridSpec
from scipy import stats
from scipy.optimize import curve_fit

# ============================================================
# PARAMÈTRES À AJUSTER
# ============================================================
BOX_SIZE     = 20.0
DT           = 0.0005
LABEL        = "Velocity Verlet, N=400"
COLOR        = "#378ADD"

# Vitesse de la vidéo : on n'affiche qu'un step sur FRAME_SKIP
FRAME_SKIP   = 50   # augmenter pour aller plus vite
FPS          = 30

POS_FILE     = "positions.csv"
VEL_FILE     = "velocities.csv"
ENERGY_FILE  = "energies.csv"
RDF_FILE     = "rdf.csv"
# ============================================================


# ------------------------------------------------------------
# 1. CHARGEMENT
# ------------------------------------------------------------
print("Chargement des données...")

df_energy = pd.read_csv(ENERGY_FILE)
df_rdf    = pd.read_csv(RDF_FILE, header=None, names=["r", "g"])

# Positions : chaque ligne = x0,y0,x1,y1,...
pos_raw = pd.read_csv(POS_FILE, header=None).values
N       = pos_raw.shape[1] // 2
steps   = pos_raw.shape[0]

# x[step, particule], y[step, particule]
x_all = pos_raw[:, 0::2]
y_all = pos_raw[:, 1::2]

# Vitesses : même format
vel_raw = pd.read_csv(VEL_FILE, header=None).values
vx_all  = vel_raw[:, 0::2]
vy_all  = vel_raw[:, 1::2]

print(f"  {steps} steps, {N} particules chargées.")


# ------------------------------------------------------------
# 2. ANIMATION : positions
# ------------------------------------------------------------
print("Génération de l'animation...")

frames = list(range(0, steps, FRAME_SKIP))

fig_anim, ax_anim = plt.subplots(figsize=(6, 6))
ax_anim.set_xlim(0, BOX_SIZE)
ax_anim.set_ylim(0, BOX_SIZE)
ax_anim.set_aspect("equal")
ax_anim.set_facecolor("#f8f8f8")
ax_anim.set_xlabel("x", fontsize=11)
ax_anim.set_ylabel("y", fontsize=11)

scat = ax_anim.scatter(x_all[0], y_all[0],
                       s=6, c=COLOR, alpha=0.7, linewidths=0)
time_text = ax_anim.text(0.02, 0.96, "", transform=ax_anim.transAxes,
                         fontsize=10, va="top")

def update(frame_idx):
    step = frames[frame_idx]
    scat.set_offsets(np.c_[x_all[step], y_all[step]])
    t = step * DT
    time_text.set_text(f"step={step}  t={t:.2f}")
    return scat, time_text

ani = animation.FuncAnimation(fig_anim, update,
                              frames=len(frames),
                              interval=1000 / FPS,
                              blit=True)

ani.save("simulation.gif", writer="pillow", fps=FPS, dpi=100)
print("  → simulation.gif sauvegardé")
plt.close(fig_anim)


# ------------------------------------------------------------
# 3. ÉNERGIES
# ------------------------------------------------------------
print("Tracé des énergies...")

t_sim = df_energy["step"] * DT

fig_e, axes_e = plt.subplots(2, 1, figsize=(9, 7), sharex=True)

axes_e[0].plot(t_sim, df_energy["Ek"],   label="Énergie cinétique",   color="#D85A30", lw=1)
axes_e[0].plot(t_sim, df_energy["Ep"],   label="Énergie potentielle", color="#1D9E75", lw=1)
axes_e[0].plot(t_sim, df_energy["Etot"], label="Énergie totale",      color=COLOR,     lw=1.5)
axes_e[0].set_ylabel("Énergie", fontsize=11)
axes_e[0].legend(fontsize=10)
axes_e[0].grid(True, ls="--", alpha=0.4)
axes_e[0].set_title(f"Énergies, {LABEL}", fontsize=12)

if "dE_rel" in df_energy.columns:
    axes_e[1].plot(t_sim, df_energy["dE_rel"] * 100, color=COLOR, lw=1.2)
    axes_e[1].axhline(0, color="gray", lw=0.8, ls="--")
    axes_e[1].set_ylabel(r"$(E_{tot}-E_0)/|E_0|$ (%)", fontsize=11)
    axes_e[1].set_title("Dérive relative de l'énergie totale", fontsize=12)
    axes_e[1].grid(True, ls="--", alpha=0.4)

axes_e[1].set_xlabel("Temps simulé", fontsize=11)
fig_e.tight_layout()
fig_e.savefig("energies.pdf", bbox_inches="tight")
print("  → energies.pdf sauvegardé")
plt.close(fig_e)


# ------------------------------------------------------------
# 4. RDF
# ------------------------------------------------------------
print("Tracé de la RDF...")

fig_r, ax_r = plt.subplots(figsize=(8, 4))
ax_r.plot(df_rdf["r"], df_rdf["g"], color=COLOR, lw=1.5, label="g(r)")
ax_r.axhline(1,     color="gray",   ls=":",  lw=0.8, label="gaz parfait")
ax_r.set_xlabel("r", fontsize=12)
ax_r.set_ylabel("g(r)", fontsize=12)
ax_r.set_title(f"Fonction de distribution radiale, {LABEL}", fontsize=12)
ax_r.legend(fontsize=10)
ax_r.grid(True, ls="--", alpha=0.4)
ax_r.set_xlim(0, df_rdf["r"].max())
fig_r.tight_layout()
fig_r.savefig("rdf.pdf", bbox_inches="tight")
print("  → rdf.pdf sauvegardé")
plt.close(fig_r)


# ------------------------------------------------------------
# 5. DISTRIBUTION DES VITESSES
# ------------------------------------------------------------
print("Tracé des distributions de vitesses...")

# On prend le dernier step (système thermalisé)
vx_final = vx_all[-1]
vy_final = vy_all[-1]

def gaussian(v, sigma_v):
    return (1 / (sigma_v * np.sqrt(2 * np.pi))) * np.exp(-v**2 / (2 * sigma_v**2))

fig_v, axes_v = plt.subplots(1, 2, figsize=(11, 4))

for ax, data, label_v, color_v in zip(
        axes_v,
        [vx_final, vy_final],
        [r"$v_x$", r"$v_y$"],
        ["#378ADD", "#1D9E75"]):

    counts, bin_edges = np.histogram(data, bins=40, density=True)
    bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])

    # Fit gaussien
    popt, _ = curve_fit(gaussian, bin_centers, counts, p0=[np.std(data)])
    sigma_fit = abs(popt[0])
    T_reduit  = sigma_fit ** 2  # T* = sigma_v² en unités réduites (m=1, kB=1)

    v_fit = np.linspace(bin_edges[0], bin_edges[-1], 300)

    ax.bar(bin_centers, counts, width=np.diff(bin_edges),
           color=color_v, alpha=0.5, label=label_v)
    ax.plot(v_fit, gaussian(v_fit, sigma_fit), color="k", lw=1.5,
            label=rf"Gaussienne ($\sigma_v={sigma_fit:.3f}$, $T^*={T_reduit:.3f}$)")
    ax.set_xlabel(label_v, fontsize=11)
    ax.set_ylabel("Densité de probabilité", fontsize=11)
    ax.legend(fontsize=9)
    ax.grid(True, ls="--", alpha=0.4)

axes_v[0].set_title(f"Distribution des vitesses, {LABEL}", fontsize=11)
axes_v[1].set_title("(step final)", fontsize=11)
fig_v.tight_layout()
fig_v.savefig("vitesses.pdf", bbox_inches="tight")
print("  → vitesses.pdf sauvegardé")
plt.close(fig_v)


# ------------------------------------------------------------
# 6. ÉVOLUTION TEMPORELLE DE LA DISTRIBUTION DES VITESSES
# ------------------------------------------------------------
print("Tracé de l'évolution temporelle des vitesses...")

# Trois instants : début, mi-simulation, fin
checkpoints = [0, steps // 2, steps - 1]
labels_t    = ["Début (hors équilibre)", "Mi-simulation", "Fin (équilibre)"]
colors_t    = ["#D85A30", "#1D9E75", "#378ADD"]

fig_vt, ax_vt = plt.subplots(figsize=(8, 4))

for idx, (step_idx, lbl, col) in enumerate(zip(checkpoints, labels_t, colors_t)):
    vx = vx_all[step_idx]
    counts, bin_edges = np.histogram(vx, bins=35, density=True)
    bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])

    ax_vt.bar(bin_centers, counts, width=np.diff(bin_edges),
              color=col, alpha=0.4, label=f"{lbl} (step {step_idx})")

    # Fit gaussien
    try:
        popt, _ = curve_fit(gaussian, bin_centers, counts, p0=[np.std(vx)])
        v_fit = np.linspace(bin_edges[0], bin_edges[-1], 300)
        ax_vt.plot(v_fit, gaussian(v_fit, abs(popt[0])), color=col, lw=1.5)
    except RuntimeError:
        pass

ax_vt.set_xlabel(r"$v_x$", fontsize=11)
ax_vt.set_ylabel("Densité de probabilité", fontsize=11)
ax_vt.set_title(f"Évolution temporelle de la distribution de $v_x$, {LABEL}", fontsize=11)
ax_vt.legend(fontsize=9)
ax_vt.grid(True, ls="--", alpha=0.4)
fig_vt.tight_layout()
fig_vt.savefig("vitesses_temporel.pdf", bbox_inches="tight")
print("  → vitesses_temporel.pdf sauvegardé")
plt.close(fig_vt)


print("\nTerminé. Fichiers générés :")
print("  simulation.gif, energies.pdf, rdf.pdf, vitesses.pdf, vitesses_temporel.pdf")