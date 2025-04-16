import matplotlib.pyplot as plt
import numpy as np

# Measured times in seconds.
# These are the reported times for each difficulty level.
sequential_times = {
    'easy': 211.703,
    'medium': 212.053,
    'hard': 859.371
}

parallel_times = {
    'easy': 31.4985,
    'medium': 32.5515,
    'hard': 137.093
}

# sequential_time / parallel_time.
difficulties = ['easy', 'medium', 'hard']
speedups = [sequential_times[d] / parallel_times[d] for d in difficulties]

# execution times.
fig, axs = plt.subplots(2, 1, figsize=(8, 10))
x = np.arange(len(difficulties))

axs[0].bar(x - 0.15, [sequential_times[d] for d in difficulties], width=0.3, label='Sequential', color='skyblue')
axs[0].bar(x + 0.15, [parallel_times[d] for d in difficulties], width=0.3, label='Parallel', color='lightgreen')
axs[0].set_xticks(x)
axs[0].set_xticklabels(difficulties)
axs[0].set_ylabel('Time (s)')
axs[0].set_title('Execution Times')
axs[0].legend()

# Speedup.
axs[1].bar(x, speedups, color='coral')
axs[1].set_xticks(x)
axs[1].set_xticklabels(difficulties)
axs[1].set_ylabel('Speedup Factor')
axs[1].set_title('Sequential vs. Parallel Speedup')

plt.tight_layout()
plt.savefig("speedup_graph.png")
plt.show()
