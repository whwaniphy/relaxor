#ifndef IMPRESOR_H
#define IMPRESOR_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>

// Imprime datos de variables double
void out(double value, std::string ARCHIVO, bool br = true);
// Imprime datos de los arreglos vectoriales
void array_print(const std::vector< int >& V, std::string ARCHIVO);
void array_print(const std::vector< double >& V, std::string ARCHIVO);
void array_print(const std::vector< double >& V, std::string ARCHIVO, unsigned int colsize, double scale);

void array_print_bin(const std::vector< int >& V, std::string ARCHIVO);
void array_print_bin(const std::vector< double >& V, std::string ARCHIVO);

// Imprime datos de los arreglos matricales
void array_print(const std::vector< std::vector<int> >& M,
		 std::string ARCHIVO);
void array_print(const std::vector< std::vector<unsigned int> >& M,
		 std::string ARCHIVO);
void array_print(const std::vector< std::vector<double> >& M,
		 std::string ARCHIVO);
void array_print_bin(const std::vector< double * >& V, std::string ARCHIVO, unsigned int cols);
void import_data(std::vector < std::vector< double > >& M,
		 std::string ARCHIVO,
                 unsigned int filas, unsigned int columnas);
#endif // IMPRESOR_H
