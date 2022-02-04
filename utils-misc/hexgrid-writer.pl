=pod

Let's write a PETSCII hexmap to a binary file.


  THIN_TOP       163
  THIN_BOTTOM    164
  FORE_SLASH     206
  BACK_SLASH     205

=cut

my @hexrow = <DATA>;

open my $fp, '>', 'HEXGRID.BIN';
print $fp pack 'xx';

for(1..1) # just do it once.  No need for more.
{
    foreach my $row (@hexrow)
    {
       my @c = split '', $row;
       foreach my $c (@c[0..75])
       {
           print $fp pack 'C', 206 if $c eq '/';
           print $fp pack 'C', 205 if $c eq '\\';
           print $fp pack 'C', 164 if $c eq '_';
           print $fp pack 'C', 32  if $c eq ' ';
#           print $fp pack 'C', 13  if $c eq '\n';
       }
       print $fp pack 'x'; # end of line
    } 
}
close $fp;


__DATA__
\           /     \           /     \           /     \           /     \   
 \         /       \         /       \         /       \         /       \  
  \       /         \       /         \       /         \       /         \ 
   \_____/           \_____/           \_____/           \_____/           \
   /     \           /     \           /     \           /     \           /
  /       \         /       \         /       \         /       \         / 
 /         \       /         \       /         \       /         \       /  
/           \_____/           \_____/           \_____/           \_____/   
