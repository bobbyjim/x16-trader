package UText;
=pod

  A text-compression engine based on the ZText format,
  but designed specifically for world names for the
  Traveller role-playing game.
  
  Differences from ZText/ZSCII
  
  * Only TWO cases are used, each with 31 entries.
  * Alphabets are switched using position 32 in each.
  
=cut

#
# The alphabets
#
my @a1 = split '', " ABCDEFGHIJKLMNOPQRSTUVWXYZ?:'!";
my @a2 = split '', " 0123456789ABCDEFGHILMNORSTUWY-";

my %a1 = map { $a1[$_] => $_ } 0..30; # don't include Z=31
my %a2 = map { $a2[$_] => $_ } 0..30; # don't include Z=31

=pod

   Encode a VERY constrained subset of ASCII to U-codes.

   Z-char 0123456789abcdef0123456789abcdef
          --------------------------------
       x   ABCDEFGHIJKLMNOPQRSTUVWXYZ?:'!y
       y   0123456789ABCDEFGHILMNORSTUWY-x
   
   x = Toggle to the 'x' character set.  Also used to pad end of string if necessary.
   y = Toggle to the 'y' character set.  Also used to pad end of string if necessary.
=cut
sub uencode
{
   my $phrase = shift;
   my $uword = uc $phrase;
#   print STDERR "\nEncoding: $uword\n";
   my %cur = %a1;
   my @out = ();
   foreach my $c (split '', $uword)
   {
      my $zvalue = 27; # space?
      if ( ! exists $cur{$c} )
      {
#	 print STDERR "Switching case (Z 31)\n";
	 %cur = %cur == %a1? %a2 : %a1;
	 push @out, 31; # toggle case
      }
      $zvalue = $cur{$c};
      push @out, $zvalue;
#      print STDERR "$c = Z $zvalue\n";
   }
   
#   print STDERR join(",", map { sprintf "%08b", $_ } @out), "\n";
#   print STDERR join(",", @out), "\n";
#   print join(",", @out), "\n";
   
   return @out;
}


=pod

   Pack a series of U-codes into bytes.
   
   Text in memory consists of a sequence of 2-byte words. 
   Each word is divided into three 5-bit U-codes, 
   plus 1 bit left over, arranged as:

   --first byte-----   --second byte------
   0 1 2 3 4   5 6 7   0 1   2 3 4 5 6   7
   --first--   --second---   --third--  END

   Final bit 7 tags the end-of-string.
   
=cut
sub upack
{
   my @source = @_; # split ',', $bytes;
   my @zbytePairs = ();
   
   {
      # grab 3 bytes from the source
      my $b1 = shift @source;
      my $b2 = shift @source;
      my $b3 = shift @source;
      
      # pack 'em into two output bytes
      my $zb1 = $b1 + ($b2 & 7) * 32;
      my $zb2 = ($b2 & 24) / 8 + $b3 * 4;

#      print STDERR "[$b1,$b2,$b3: $zb1, $zb2]"; 
      push @zbytePairs, [$zb1, $zb2];
      redo if @source;
   }
#   print STDERR "\n";

   my $last = pop @zbytePairs;
   $last->[1] |= 128; # mark last byte
   push @zbytePairs, $last;
   
   my @out = ();
   foreach my $pair (@zbytePairs)
   {
      push @out, $pair->[0], $pair->[1];
   }
   
   return @out;
}

#
#  Wrapper method.  Encodes an ASCII string into U-code
#  and then packs that into 16-bit integers.
#
sub convert
{
   my $ascii = uc shift;
   return upack( uencode( $ascii ) );
}


1; # return 1 as all good Perl modules should
