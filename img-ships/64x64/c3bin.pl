#!/usr/bin/env perl
#
#  Takes multiple PNG files as arguments, sends each through
#  png2sprite.py, and writes them as a single contiguous binary file, 
#  complete with initial two null bytes, ready to load.
#

my $outfile = shift || die "SYNOPSIS: $0 OUTFILE [PNG file...]\n";
open my $fp, '>', uc $outfile;
print $fp pack 'xx';

foreach my $pngfile (@ARGV)
{
   my $tmpfile = "$pngfile.tmp";
   $pngfile =~ s/\.BIN//i;
   $pngfile .= '.PNG' unless $pngfile =~ /\.PNG/i;
   print STDERR "processing $pngfile\n";

   my $ret = system './png2sprite.py', $pngfile, $tmpfile;
   die "ERROR invoking png2sprite.py on [$pngfile]\n" if $ret;

   open my $in, '<', $tmpfile || die "ERROR: could not open tempfile '$tmpfile'\n";
   for(<$in>)
   {
      next unless /0x/;
      chomp;
      s/0x/\$/g;
      s/,$//;
      s/\$//g; # remove dollar sign please
   
      my @numbers = map { hex $_ } split /,/; # these should all be nice and hexy

      print $fp pack 'C*', @numbers;
   }

   close $in;
   unlink $tmpfile;
}
close $fp;
