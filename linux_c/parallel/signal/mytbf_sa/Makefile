all:mytbf_sa
mytbf_sa:main.o mytbf.o
	gcc $^ -o $@ -lrt

clean:
	rm -rf *.o mytbf_sa