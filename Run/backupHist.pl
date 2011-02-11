#!/usr/bin/perl

use strict;

my $setSize;

my $line;
my $setNo;
my $lineNo;

my @distances;
my @sortedDistances;

my @toks;

my $i;
my $j;

my $sum;

if(scalar(@ARGV) != 1)
  {
    die "format: perl backupHist. pl <setSize>";
  }

$setSize = $ARGV[0];

print STDERR "setSize is $setSize\n";

$lineNo = -1;
$setNo = -1;

while($line = <STDIN>)
  {
    $lineNo++;

    if($lineNo % $setSize == 0)
      {
	# New set.
	
	$setNo++;

	print STDERR "line $lineNo set $setNo\n";
      }

    @toks = split(/[\ \t\n]+/, $line);

    push(@distances, sqrt($toks[0] * $toks[0] + $toks[1] * $toks[1]));
  }

if(($lineNo + 1) % $setSize != 0)
{
    die "number of lines not a multiple of setSize";
}

for($i = 0; $i <= $setNo; $i++)
{
    print STDERR "sorting set $i\n";

    @sortedDistances = sort {$a <=> $b} splice(@distances, 0, $setSize);
    $sum = 0;

    for($j = 0; $j < $setSize; $j++)
    {
	$sum += $sortedDistances[$j];
	print "sorted $i $j $sortedDistances[$j] $sum\n";
    }
}



# #!/usr/bin/perl

# # updated May 10, 2007
# #
# # This script calculates accuracy, false positive, and false
# # negative rates given a tabluar input file. It assumes that
# # columns startCol to endCol of every row in the file contain
# # successive rows of the confusion matrix (i.e., each row 
# # of the input file refers to some run).

# use strict;

# my $classes; # number of classes

# my $startc; # start column
# my $endc; # end column
# my $numc; # number of columns

# my $line;
# my @toks;

# my $i;
# my $j;

# my $TN;
# my $TP;
# my $FN;
# my $FP;

# my $index;

# if(scalar(@ARGV) != 3)
# {
#     die "format: perl accDetFpr.pl <numClasses> <startCol> <endCol> < <file>";
# }

# $classes = $ARGV[0];
# $startc = $ARGV[1];
# $endc = $ARGV[2];

# $numc = $endc - $startc + 1;

# if($classes < 2)
# {
#     die "not enough classes";
# }

# if($numc != $classes * $classes)
# {
#     die "<numClasses> x <numClasses> != <endCol> - <startCol> + 1";
# }

# # print header

# print "line ";

# for($i = 0; $i < $classes; $i++)
# {
#     for($j = 0; $j < $classes; $j++)
#     {
# 	if($i != 0 || $j != 0)
# 	{
# 	    print " ";
# 	}

# 	print $i . "as" . $j;
#     }
# }

# for($i = 0; $i < $classes; $i++)
# {
#     print " ACC" . $i . " DET" . $i . " FPR" . $i;
# }
# print "\n";

# $index = 0;

# while($line = <STDIN>)
# {
#     print $index++ . " ";

#     @toks = split(/[\ \t\n]+/, $line);
    
#     for($i = 0; $i < $startc; $i++)
#     {
# 	shift(@toks);
#     }

#     while(scalar(@toks) > $numc)
#     {
# 	pop(@toks);
#     }

#     # print counts

#     for($i = 0; $i < $numc; $i++)
#     {
# 	if($i != 0)
# 	{
# 	    print " ";
# 	}
	
# 	print $toks[$i];
#     }

#     # print rates
    
#     for($i = 0; $i < $classes; $i++)
#     {
# 	$TP = $FP = $TN = $FN = 0;

# 	# true positives -- patterns of class i correctly labeled as i

# 	$TP = $toks[$i * $classes + $i];
	
# 	# false positives -- patterns not of class i labeled as i

# 	for($j = 0; $j < $classes; $j++) # all patterns labeled as i
# 	{
# 	    $FP += $toks[$j * $classes + $i];
# 	}
	
# 	$FP -= $TP; # subtract out patterns correctly labeled as i

# 	# true negatives -- patterns not of class i labeled as not i

# 	for($j = 0; $j < $numc; $j++) # sum all
# 	{
# 	    $TN += $toks[$j];
# 	}

# 	for($j = 0; $j < $classes; $j++) # subtract out all of class i
# 	{
# 	    $TN -= $toks[$i * $classes + $j];
# 	}

# 	$TN -= $FP; # subtract out FP

# 	# false negatives -- patterns of class i not labeled as i

# 	for($j = 0; $j < $classes; $j++) # sum all class i
# 	{
# 	    $FN += $toks[$i * $classes + $j];
# 	}

# 	$FN -= $TP; # subtract all correctly classified as i

# 	# acc
	
# 	print " " . sprintf("%0.4f", ($TN + $TP) / ($TN + $TP + $FN + $FP));

# 	# det

# 	print " " . sprintf("%0.4f", ($TP) / ($FN + $TP));

# 	# fpr
	
# 	print " " . sprintf("%0.4f", ($FP) / ($TN + $FP));
#     }

#     print "\n";
# }
