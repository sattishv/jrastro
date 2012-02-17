JRastro
=======

JRastro is a tracing library for Java applications based on the JVMTI
interface. The library is released under GPLv3, and requires
[librastro](https://github.com/schnorr/akypuera/tree/master/librastro)
to provide a low memory footprint and low intrusion. The binary trace
files are converted to the Paje file format using the tool called
`jrastro_converter`.

Compiling
---------

    $ git clone git@github.com:schnorr/jrastro.git
    $ mkdir jrastro/build ; cd build
    $ cmake -DRASTRO_PATH:PATH=/usr/local/ ..
    $ make


Using
-----

JRastro is a JVMTI agent, so you can invoke it as you'll do for any
other JVMTI agent. Here's a way to do it:

    $ java -agentpath:/path/to/your/libjrastro.so YourJavaApplication

You have four environment variables that you need to set before
launching your application with the JRastro agent:

* JRST_THREAD_TRACING : Traces the start and end of Java threads
* JRST_METHOD_TRACING : Requires the first, traces the beginning and end of all methods
* JRST_MONITOR_TRACING : Traces monitors of Java threads
* JRST_GC_TRACING : Traces the garbage collector (when it is active, when it is not)

Depending on the existence of these environment variables, the agent
will trace the application creating a number of trace
files. Generally, it creates one trace file per thread present in the
application, plus one trace file to register the monitor behavior,
another for the garbage collector. A run with three threads generate
may generate this list of files:

    rastro-0-0.rst
    rastro-1-0.rst
    rastro-2-0.rst

Filtering classes and methods
-----------------------------

The user can define the environment variable `JRST_FILTER` with the
name of a text file that will be used as a filter of method tracing.
This file needs to be in the following format to be accepted:

    $ export JRST_FILTER=filter
    $ cat filter
       C SimpleThread
       M sleepForThreeSeconds
       M doTheJob

Only the classes (and all their methods) and the methods that appear
in this file will be traced. Multiple classes and methods are
accepted. If the file is not provided through the `JRST_FILTER`
environment variable, __all__ classes and methods of the JVM will be
traced, and you will have huge trace files.

jrastro_converter
-----------------

Trace files created with JRastro are converted to the [Paje generic
format](http://paje.sf.net) using the `jrastro_converter`, passing as
parameter __all the rst__ files generated by the mpi execution.  The
corresponding paje trace file is printed on __stdout__.

    $ jrastro_converter rastro*.rst