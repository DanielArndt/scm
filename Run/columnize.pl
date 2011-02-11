#!/usr/bin/perl

# This script takes a tabular file and turns one long column into several
# columns. Specifically, it considers the first <rows> elements in column
# <column> in the original file and makes that the first column, takes
# the next <rows> elements in column <column> in the original file and 
# makes that the second column, takes the next <rows> elements etc.

use strict;

my $rows; # How many rows the processed columns are to be.
my $column; # The column from the original file to extract.

my $line;
my @toks;

my @colVals; # The values in the column to be extracted.

my $numCols; # Number of processed columns.

my $i;
my $j;

if(scalar(@ARGV) != 2)
  {
    die "format: perl columnize.pl <rows> <column>";
}

$rows = $ARGV[0];
$column = $ARGV[1];

# Collect the values in column <column> storing them into @colVals.
while($line = <STDIN>)
  {
    @toks = split(/[\ \t\n]+/, $line);
    
    push(@colVals, $toks[$column]);
  }

# Get the number of columns to be printed out, make sure there is no elements left over.

if(scalar(@colVals) % $rows != 0)
  {
    die "bad number of rows ($rows) in the final file (only " . scalar(@colVals) . " rows in total)";
  }

$numCols = scalar(@colVals) / $rows;

for($i = 0; $i < $rows; $i++)
  {
    # Consider each row in the processed columns together.

    for($j = 0; $j < $numCols; $j++)
      {
	print $colVals[$j * $rows + $i] . " ";
      }
    print "\n";
  }







