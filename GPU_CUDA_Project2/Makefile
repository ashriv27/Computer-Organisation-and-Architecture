all: app0 app1 app2 app3 app4 app5 app6

app0:
	g++ matrixMul_cpu.cpp -o matrixMul_cpu.exe

app1:
	nvcc matrixMul_gpu_part1.cu -o matrixMul_gpu_part1.exe

app2:
	nvcc matrixMul_gpu_part2.cu -o matrixMul_gpu_part2.exe

app3:
	nvcc matrixMul_gpu_part3.cu -o matrixMul_gpu_part3.exe

app4:
	nvcc matrixMul_gpu_part4a.cu -o matrixMul_gpu_part4a.exe

app5:
	nvcc matrixMul_gpu_part4b.cu -o matrixMul_gpu_part4b.exe

app6:
	nvcc matrixMul_gpu_part4b_prefetch.cu -o matrixMul_gpu_part4b_prefetch.exe

clean:
	rm -rf *.exe
