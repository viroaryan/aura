// JavaScript Speed Benchmark Test
// Running 100 Million Iterations in a Loop

console.log("JavaScript (Node.js) Benchmark: Starting 2,000,000,000 iterations...");
const start = performance.now();

let i = 0;
let count = 0;
while (i < 2000000000) {
    count++;
    i++;
}

const end = performance.now();
const elapsed = (end - start) / 1000;

console.log("JavaScript Benchmark Finished!");
console.log(`Final Count: ${count}`);
console.log(`Time taken by JavaScript: ${elapsed.toFixed(4)} seconds`);
