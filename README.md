# General

This is a very simple `MPI` based framework for master/slave programs through I/O.

* It is designed for the class of programs that have a high computational cost and require large input data, such as multi-scale modeling with the aid of representative volume elements.
* The entire communication is done through disk by communicating a configuration file path through the network. This cost should be justifiable for this approach to be effective.
* The gathering and post processing can also be done through the disk if needed.

# Usage
The usage of this code is pretty simple and only requires derivation from its bases classes
1) Derive a class from `MPIMasterProcess` class and define the function `createTasks_` that creates a series of `MPITask` objects to work with. The `MPITask` need to contain the path to a configuration file that need to be parsed at each processors to provide information on how to reinterpret the data. 
2) Derive a class from `MPISlaveProcess` class and define the function `executeTask_(const std::string& path)`. This function generally should read the input file from `path` variable, parse it and execute the corresponding program with specific configuration. At the end of the execution if needed, the program may store the output in a specific file to be post processed at the end of the program by master.
3) Define an `MPIProgram<DerivedMasterClass, DerivedSlaveClass>` object and start the MPI process by invoking `program.run()`

# Remarks
* If disk communication is not desirable a set of data can be packed into an array of `const char` as bytes and send through the path variable of `MPITask`. This data need to be unpacked at `executeTask_(const std::string& path)` function at slave processor.
* The input arguments can be accessed to modify the code based on runtime variables through `void* data` and `int dataSize` if needed.
* The post processing can be defined on master process by overriding the optional postProcess_() function to enable gathering mechanism. This function will be called after the entire taskpool the tasks are completed.

# Example
A test example for calculating the value of PI based on the integration of a quarter of a circle is also included in the package to demonstrate how the program is intended to use. Please note that this example due to its low computational cost and small input is very inefficient to be used in this framework, however, it is very simple to show how the program is intended to use.

* The program divides the integral domain (0 , 1) into 500 parts and define a task for each part. 
* The tasks are then scatter these tasks between slave processors.
* The slave processors calculate the integral based on 10000000 subdivision
* The result on each slave processors is saved in an output file
* The Master processor will compute the results by summing each fracture at the end of the simulation

The code can be compiled by `make all` and invoked by either `make run` or `mpiexec -np <n> ./a.out”` in which `<n>` is the number of processors.
