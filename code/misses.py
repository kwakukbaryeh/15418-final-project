import matplotlib.pyplot as plt

# Data for cache misses
difficulties = ['Easy', 'Medium', 'Hard']
sequential_cache_misses = [6_395_607, 6_427_554, 11_238_606]
parallel_cache_misses   = [12_779_811, 12_004_482, 34_762_212]

x = range(len(difficulties))

plt.figure(figsize=(8, 6))
plt.bar(x, sequential_cache_misses, width=0.4, label='Sequential', align='center')
plt.bar([i + 0.4 for i in x], parallel_cache_misses,   width=0.4, label='Parallel',   align='center')

plt.xlabel('Difficulty Level')
plt.ylabel('Cache Misses')
plt.title('Cache Misses by Difficulty (Sequential vs Parallel)')
plt.xticks([i + 0.2 for i in x], difficulties)
plt.legend()
plt.grid(axis='y', linestyle='--', alpha=0.6)
plt.tight_layout()

plt.savefig('cache_misses.png', dpi=300)


