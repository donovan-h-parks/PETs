Network Diversity
=================

Calculation of beta diversity over phylogenetic networks.

-------------------------------------------------------------------------------

Network Diversity is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Network Diversity is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Network Diversity.  If not, see <http://www.gnu.org/licenses/>.


Installation:
-------------------------------------------------------------------------------

Network Diversity is a command-line program written in C++.  To install 
Network Diversity, download and uncompress it with the unzip command:
  unzip NetDiv_1_0_0.zip

To compile Network Diversity on OSX or Linux simply type 'make' from within 
the source directory of Network Diversity. The resulting executable will be 
in the bin directory. A precompiled executables for Windows is provided in 
the bin directory. Please note that even under Windows, Network Diversity must 
be run from the command-line (i.e., the DOS prompt).


Program usage:
-------------------------------------------------------------------------------

Usage: Network Diversity [OPTIONS]
Calculates taxon- and phylogenetic-basec beta diversity measures.

Options:
 -h, --help           Produce help message.
 -l, --list-calc      List all supported calculators.
 -u, --unit-tests     Execute unit tests.
 
 -c, --calculator     Desired calculator (e.g., Bray-Curtis, Canberra).
 -w, --weighted       Indicated if sequence abundance data should be used.
 -y, --count          Use count data as opposed to relative proportions.

 -n, --nexus-file     Nexus input file (must contain a Taxa and Splits block, root split system with Outgroup taxa).
 -t, --newick-file    Newick input file (tree treated as implicitly rooted).
 -s, --sample-file    Sample file indicating number of times each sequences is found in a sample.
 -o, --output-file    Output file.

 -x, --max-data-vecs  Maximum number of profiles (data vectors) to have in memory at once (default = 1000).

 -v, --verbose        Provide additional information on program execution.

Examples of Use:
 ./NetworkDiversity -n input.nex -s seq.txt -o output.txt -c Bray-Curtis -w
 ./NetworkDiversity -t input.tre -s seq.txt -o output.txt -c Bray-Curtis -w
  
Verifying software installation:
-------------------------------------------------------------------------------

A set of unit tests is included to verify proper installation of the Network 
Diversity software. The unit tests can be run with:
  ./NetworkDiversity -u

The software should not be used if any of the unit tests fail.

Supported beta-diversity measures:
-------------------------------------------------------------------------------

A list of supported measures (calculators) can be obtained with the -l flag:
  ./NetworkDiversity -l
  
This list is also available at:
  http://kiwi.cs.dal.ca/Software/Supported_beta-diversity_Measures
 
Input file formats:
-------------------------------------------------------------------------------

Network Diversity uses Newick trees and Nexus split systems as input. Information 
on the Newick tree format can be found at: 
  http://evolution.genetics.washington.edu/phylip/newicktree.html.
  
Here is a simple Newick tree with three leaf nodes labelled A, B, and C:
 (A:1,(B:1,C:1):1);
 
Trees or split systems containing incompatible splits can be specified using
the Nexus format. Network Diversity is compatible with Nexus files generated
by SplitsTree (http://www.splitstree.org/). In particular, Network Diversity
requires a Taxa and Splits block.

Sequence count information must be specified as a tab-delimited table where
each row is a sample and each column is the name of a leaf node in the provided
tree. Data must be provided for all leaf nodes in the tree. Consider the 
following example:

  A	B	C
Sample1	1	2	3
Sample2	10	1	0
Sample3	0	0	1

The first row is a tab seperate list of the taxa present in the tree or split 
system. Please note that this line MUST start with a tab. The number of sequences 
associated with each taxa is then indicated for each sample on a seperate
row. In this example, the first sample is labelled 'Sample1' and contains 1
instance of taxa A, 2 instances of B, and 3 instances of C. Sample3
contains only instances of C, but note that zeros must be specified for the
other taxa.

Example input files are avaliable in the unit-tests directory. 

Dissimilarity output file format:
-------------------------------------------------------------------------------

The resulting dissimilarity between samples is written as a tab-delimited, 
lower-triangular dissimilarity matrix with the first line indicating the number
of samples. Consider the following output:

3
A
B	1
C	2	3

The first line indicates that there are 3 samples. The dissimilarity between 
samples A and B is 1, A and C is 2, and B and C is 3.

Rooting phylogenies:
-------------------------------------------------------------------------------

For Newick trees, Network Diversity assumes the tree is implicitly rooted. If
convenient outgroup taxa can be contained in the tree. In this case, the
sample file should specify which taxa are from the outgroup:

	A	B	C	O1	O2
Sample1	1	2	3	0	0
Sample2	10	1	0	0	0
Sample3	0	0	1	0	0
Outgroup	0	0	0	1	1

The outgroup sample must be labelled 'Outgroup' or 'outgroup'. Network Diversity 
assumes the outgroup taxa for a monophyletic group and that the tree is rooted
between the outgroup and ingroup.

Nexus split systems are assumed to be unrooted unless an outgroup is specified.
As above, the sample file must contain a sample labelled 'Outgroup' or 'outgroup'
which indicates which taxa are from the outgroup. The outgroup does not need to
be monophyletic. Any split with outgroup taxa in both induced subsets is
ignored. Please note that only root invariant measures should be applied to
an unrooted split system.

Missing taxa:
-------------------------------------------------------------------------------

Taxa in the sample file which are not in the phylogeny are ignored. This allows
beta diversity to be calculated over specific portions of the phylogeny without
changing the sample file (i.e., a particular lineage).

Similarily, taxa in a phylogeny which are not found in the sample file are
ignored. This allows general-purpose phylogenies to be easily used. For example,
sequences might be mapped to their closes proxy in a phylogeny built from the
core 16S Greengenes sequences. Such a file can be obtained from the Fast UniFrac
website:

http://bmf2.colorado.edu/fastunifrac/tutorial.psp


Citing Network Diversity:
-------------------------------------------------------------------------------

If you use Network Diversity in your research, please cite:

Parks, D.H. and Beiko, R.G. Measuring community similarity with phylogenetic networks. 
  (submitted to Molecular Biology and Evolution, April, 2012).


Contact Information:
-------------------------------------------------------------------------------

Donovan Parks
parks@cs.dal.ca

Robert Beiko
beiko@cs.dal.ca

Program website: http://kiwi.cs.dal.ca/Software/Network_Diversity
