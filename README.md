PokerSim
===============================
<p>
Given a single player's hand, runs simple Monte Carlo Texas Hold 'Em simulations to determine the likeliest relative strength and the equity of the given hand. </p>
<p>
NOTE: will only compile on POSIX machines as it uses the pthreads library to parallelize the running of simulations. </p>
<p>
NOTE: requires the PokerSource library be installed (pokersource.sourceforge.net) -- assumes default installation directory (/usr/local/) but this can easily be configured in the make file. </p>

##Compilation
<p>Given the notes above (i.e., given a POSIX compliant environment and the Makefile appropriately configured to point to PokerSource library), simple execute the command: 
* make all
</p>
##Usage
<p>Run with:
<p>./bin/hand_evaluator c -p nPlayers (>= 1, defaults to 4) -s nSimulations (>= 1, defaults to 10,000,000) -t nThreads (>= 1, defaults to 4)_</p>
When prompted, enter card strings in the following format: RankSuite, where
* Rank is a character among 2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K, A
* Suite is a character among h, d, c, s
<p>E.g., "Ah2d" is the appropriate string to represent that hand consisting of the ace of heards and the two of diamonds.</p>
</p>
