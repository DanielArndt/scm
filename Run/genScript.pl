#!/usr/bin/perl

use strict;

# uncomment one env only

#my $envType = "datasetEnv";
#my $envType = "rubikEnv";
my $envType = "backupEnv";

# dataset env
my $trainSetName = "ann-train.data";
my $testSetName = "ann-test.data";
my $trainSetSize = 3772;
my $testSetSize = 3428;
my $setDim = 21;

# these apply to both rubik and backup env
my $numTestPoints = 10;
my $numValidPoints = 10;
my $useBinaryFitness = 1;

# rubik env
my $rubikInitTwists = 10;
my $rubikMutateTwists = 3;

# backup env
my $mux = 5;
my $muy = 10;
my $muthetas = 18;
my $muthetac = 18;

# scm params

my $exe = "scm";

my $seed = 0;

my $paretoEpsilonPoint = 0.001;
my $paretoEpsilonTeam = 0.001;

my $pointPow = 3;
my $teamPow = 3;

my $maxSteps = 20;

my $Psize = 16;
my $Msize = 16;
my $pmd = 0.3;
my $pma = 0.4;
my $pmm = 0.1;
my $pmn = 0.1;
my $omega = 10;
my $t = 100;
my $numLevels = 2;
my $Pgap = 8;
my $Mgap = 2;

my $maxProgSize = 48;
my $pBidMutate = 1.0;
my $pBidSwap = 1.0;
my $pBidDelete = 0.5;
my $pBidAdd = 0.5;

my $statMod = 5;

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

    print ARG "envType $envType\n";
    print ARG "\n";

    print ARG "trainSetName $trainSetName\n";
    print ARG "testSetName $testSetName\n";
    print ARG "trainSetSize $trainSetSize\n";
    print ARG "testSetSize $testSetSize\n";
    print ARG "setDim $setDim\n";
    print ARG "\n";

    print ARG "numTestPoints $numTestPoints\n";
    print ARG "numValidPoints $numValidPoints\n";
    print ARG "useBinaryFitness $useBinaryFitness\n";
    print ARG "\n";

    print ARG "rubikInitTwists $rubikInitTwists\n";
    print ARG "rubikMutateTwists $rubikMutateTwists\n";
    print ARG "\n";

    print ARG "mux $mux\n";
    print ARG "muy $muy\n";
    print ARG "muthetas $muthetas\n";
    print ARG "muthetac $muthetac\n";
    print ARG "\n";

    print ARG "seed $nextSeed\n";
    print ARG "\n";

    print ARG "paretoEpsilonPoint $paretoEpsilonPoint\n";
    print ARG "paretoEpsilonTeam $paretoEpsilonTeam\n";
    print ARG "pointPow $pointPow\n";
    print ARG "teamPow $teamPow\n";
    print ARG "maxSteps $maxSteps\n";
    print ARG "\n";

    print ARG "Psize $Psize\n";
    print ARG "Msize $Msize\n";
    print ARG "pmd $pmd\n";
    print ARG "pma $pma\n";
    print ARG "pmm $pmm\n";
    print ARG "pmn $pmn\n";
    print ARG "omega $omega\n";
    print ARG "t $t\n";
    print ARG "numLevels $numLevels\n";
    print ARG "Pgap $Pgap\n";
    print ARG "Mgap $Mgap\n";
    print ARG "\n";

    print ARG "maxProgSize $maxProgSize\n";
    print ARG "pBidMutate $pBidMutate\n";
    print ARG "pBidSwap $pBidSwap\n";
    print ARG "pBidDelete $pBidDelete\n";
    print ARG "pBidAdd $pBidAdd\n";
    print ARG "\n";

    print ARG "statMod $statMod\n";
    
    close(ARG);
    
    print RUN "./$exe $argFile 1> $prefix.$nextSeed.std 2> $prefix.$nextSeed.err\n";
}

close(RUN);
