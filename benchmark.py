# Python Speed Benchmark Test
# Running 100 Million Iterations in a Loop

import time

print("Python Benchmark: Starting 100,000,000 iterations...")
start_time = time.time()

i = 0
count = 0
while i < 100000000:
    count += 1
    i += 1

end_time = time.time()
elapsed = end_time - start_time

print("Python Benchmark Finished!")
print(f"Final Count: {count}")
print(f"Time taken by Python: {elapsed:.4f} seconds")
