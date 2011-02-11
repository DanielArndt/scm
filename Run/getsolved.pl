#!/usr/bin/perl

use strict;
use warnings;

my $i;
my $j;

my $wcout;
my @toks;

my @files;

my $cmd;

print STDERR "usage: getsolved.pl <inputfiles> <levels>\n";

print STDERR "input files is $ARGV[0], there are $ARGV[1] levels\n";

open(FILE, "<$ARGV[0]");
@files = <FILE>;
close(FILE);

for($j = 0; $j < $ARGV[1]; $j++)
{
		for($i = 0; $i < scalar(@files); $i++)
		{
				chomp($files[$i]);

				$cmd = "grep \' rtest 0 \' $files[$i] | grep ' solved 1 ' | grep \'lev $j\' | grep estate | grep -v best | cut -d \' \' -f 11 | sort -n | uniq | wc";
				print STDERR $cmd . "\n";

				$wcout = `$cmd`;
			 
				@toks = split(/[\ \t\n]+/, $wcout);
				print "$files[$i] $j $i $toks[1]\n";
		}
}
