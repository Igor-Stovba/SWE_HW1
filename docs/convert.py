import json
import sys

def convert_json_to_txt(json_file, txt_file):
    with open(json_file, "r", encoding="utf-8") as f:
        data = json.load(f)
    
    with open(txt_file, "w", encoding="utf-8") as f:
        f.write("Benchmark Report\n")
        f.write("=" * 50 + "\n\n")
        
        for entry in data["benchmarks"]:
            f.write(f"Algorithm: {entry['name']}\n")
            f.write(f"Nodes: {entry.get('Nodes', '-')}\n")
            f.write(f"Edges: {entry.get('Edges', '-')}\n")
            f.write(f"CPU Time: {round(entry['cpu_time'], 2)} ms\n")
            f.write(f"Real Time: {round(entry['real_time'], 2)} ms\n")
            f.write(f"Allocations per Iteration: {round(entry.get('allocs_per_iter', 0), 2)}\n")
            f.write(f"Max Used (Bytes / MBytes): {entry.get('max_bytes_used', '-')} / {entry.get('max_bytes_used', '-') / 1e+6}\n")
            f.write("-" * 50 + "\n\n")
    
    print(f"TXT файл {txt_file} создан успешно!")

if __name__ == "__main__":
    bench_result_file = sys.argv[1]
    bench_report_file = sys.argv[2]
    print(bench_result_file)
    convert_json_to_txt(bench_result_file, bench_report_file)
