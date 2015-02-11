$Id: README.txt,v 1.3 2013/09/27 01:00:03 glp Exp $

Metro II 1.0.alpha

Embedded system design and is becoming very difficult because of more
complex designs, shorter time-to-market, and the virtualization of
design teams.  The platform-based design (PBD) methodology addresses
these challenges by separating concerns into an architectural platform
and the corresponding functionality.  A previous software package, the
Metropolis design framework, implemented at University of California,
Berkeley, implements PBD and the orthogonilization of concerns.
Lessons learned from the Metropolis indicate the need for three new
features: support importation of pre-designed intellectual property,
separating cost from behavior when carrying out design and exploring
the design space in a structured manner. Metro II is a rewrite of
Metropolis that will included these three new features. 

INSTALL
=======

- Run "./configure" in the current directory to setup make variables. Please specify the directory of SystemC if needed. For example:  

    ./configure --with-systemc=/home/lguo/Project/systemc-2.3.1 --with-systemc-lib=lib-linux64

- Compile source by running "make" in src/
- Compile examples in examples/producer-consumer and examples/producer-consumer 

ADDITIONAL INFORMATION
======================

See also http://embedded.eecs.berkeley.edu/metropolis


