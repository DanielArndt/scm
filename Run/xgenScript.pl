#!/usr/bin/perl

use strict;

my $exe = "scm";

my $seed = 0;

my $paretoEpsilonPoint = 0.001;
my $paretoEpsilonTeam = 0.001;

my $pointPow = 3;
my $teamPow = 3;

my $maxSteps = 20;

my $envType = "rubikEnv";
my $Psize = 16;
my $Msize = 16;
# my $pd = 0.1;
# my $pa = 0.1;
# my $mua = 0.1;
my $pmd = 0.3;
my $pma = 0.4;
my $pmm = 0.1;
my $pmn = 0.1;
my $omega = 24;
my $t = 100;
my $Pgap = 4;
my $Mgap = 8;
my $pst = 0.2;

my $maxProgSize = 48;
my $pBidMutate = 0.9;
my $pBidSwap = 0.9;
my $pBidDelete = 0.9;
my $pBidAdd = 0.5;

my $mux = 5;
my $muy = 10;
my $muthetas = 18;
my $muthetac = 18;

my $statMod = 10;

if(scalar(@ARGV) != 2)
{
    die "usage: genScript.pl prefix numRuns";
}

my $prefix = $ARGV[0];
my $numRuns = $ARGV[1];

print "prefix: $prefix\n";
print "numRuns: $numRuns\n";

my $run;
my $argFile;
my $nextSeed;

open(RUN, ">$prefix" . ".run") || die "cannot open RUN $prefix.run";

for($run = 0; $run < $numRuns; $run++)
{
    $nextSeed = $seed+ $run;

    $argFile = "$prefix.$nextSeed.arg";
    
    open(ARG, ">$argFile") || die "cannot open $argFile";

    print ARG "seed $nextSeed\n";
    print ARG "\n";

    print ARG "paretoEpsilonPoint $paretoEpsilonPoint\n";
    print ARG "paretoEpsilonTeam $paretoEpsilonTeam\n";
    print ARG "pointPow $pointPow\n";
    print ARG "teamPow $teamPow\n";
    print ARG "maxSteps $maxSteps\n";
    print ARG "\n";

    print ARG "envType $envType\n";
    print ARG "Psize $Psize\n";
    print ARG "Msize $Msize\n";
#     print ARG "pd $pd\n";
#     print ARG "pa $pa\n";
#     print ARG "mua $mua\n";
    print ARG "pmd $pmd\n";
    print ARG "pma $pma\n";
    print ARG "pmm $pmm\n";
    print ARG "pmn $pmn\n";
    print ARG "omega $omega\n";
    print ARG "t $t\n";
    print ARG "Pgap $Pgap\n";
    print ARG "Mgap $Mgap\n";
    print ARG "pst $pst\n";
    print ARG "\n";

    print ARG "maxProgSize $maxProgSize\n";
    print ARG "pBidMutate $pBidMutate\n";
    print ARG "pBidSwap $pBidSwap\n";
    print ARG "pBidDelete $pBidDelete\n";
    print ARG "pBidAdd $pBidAdd\n";
    print ARG "\n";

    print ARG "mux $mux\n";
    print ARG "muy $muy\n";
    print ARG "muthetas $muthetas\n";
    print ARG "muthetac $muthetac\n";
    print ARG "\n";
    
    print ARG "statMod $statMod\n";
    
    close(ARG);
    
    print RUN "./$exe $argFile 1> $prefix.$nextSeed.std 2> $prefix.$nextSeed.err\n";
}

close(RUN);

# # The variables in UPPERCASE can be changed by the user.

# my $COMMAND = "java EvoD"; # The EvoD program file name minus the arguments, e.g., "java EvoD" or "./a.out".

# my $RUNS = 50; # Number of different initializations per configuration, should be at least 30.

# my $POPSIZE = 250; # Population size.
# my $MININDSIZE = 1; # Minimum individual size.
# my $MAXINDSIZE = 40; # Maximum individual size.
# my $MAXITER = 10000; # Number of iterations (tournaments).
# my $PX = 0.9; # Probability of crossover.
# my $PM = 0.5; # Probability of mutation.
# my $PS = 0.5; # Probability of swap.
# my $TRAINDATA = "breast.data.train"; # Training data.
# my $TESTDATA = "breast.data.test"; # Test data.
# my $RULEPOOL = "Rules_Pool.txt"; # Pool of rules.

# my $OUTFILE = "EvoD.results"; # Where to write summary of results.
# # Each line will contain the identifier and seed for the run as well as the train hits, test hits, 
# # and size corresponding to the best individual in the run (in that order).

# my $run;
# my $prefix;
# my $indSize;

# my $line;
# my @toks;

# open(OUTFILE, ">$OUTFILE");

# # Perform the variable-size runs.

# for($run = 0; $run < $RUNS; $run++)
# {
#     $prefix = "var_$run";
#     print "$COMMAND $prefix $POPSIZE $MININDSIZE $MAXINDSIZE $MAXITER $PX $PM $PS $TRAINDATA $TESTDATA $RULEPOOL $run\n";
#     print STDERR `$COMMAND $prefix $POPSIZE $MININDSIZE $MAXINDSIZE $MAXITER $PX $PM $PS $TRAINDATA $TESTDATA $RULEPOOL $run`;

#     open(INFILE, "<$prefix.best");
#     $line = <INFILE>;
#     print OUTFILE "$prefix $run $line";
#     close(INFILE);
# }

# # Perform the fixed-size runs.

# for($indSize = $MININDSIZE; $indSize <= $MAXINDSIZE; $indSize++)
# {
#     for($run = 0; $run < $RUNS; $run++)
#     {
# 	$prefix = "fix_" . $indSize . "_$run";
# 	print "$COMMAND $prefix $POPSIZE $indSize $indSize $MAXITER $PX $PM $PS $TRAINDATA $TESTDATA $RULEPOOL $run\n";
# 	print STDERR `$COMMAND $prefix $POPSIZE $indSize $indSize $MAXITER $PX $PM $PS $TRAINDATA $TESTDATA $RULEPOOL $run`;

# 	open(INFILE, "<$prefix.best");
# 	$line = <INFILE>;
# 	print OUTFILE "$prefix $run $line";
# 	close(INFILE);
#     }
# }

# close(OUTFILE);


