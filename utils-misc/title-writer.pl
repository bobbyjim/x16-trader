=pod

Let's convert ASCII to PETSCII in a binary file.

=cut

my @hexrow = <DATA>;

open my $fp, '>', 'BT-TITLE.BIN';
print $fp pack 'xx';

foreach my $row (@hexrow)
{
   my @c = split '', $row;
   foreach my $c (@c[0..50])
   {
     my $b = 0;
     $b = 206 if $c eq '/';
     $b = 205 if $c eq '\\';
     $b = 194 if $c eq '|';   # or 221
     $b = 192 if $c eq '-';
     $b = 164 if $c eq '_';
     $b = 32  if $c eq ' ';
     $b = 13  if $c eq '\n';
     $b = 39  if $c eq '\'';
     $b = 39  if $c eq '`';
     $b = 46  if $c eq '.';
     $b = 58  if $c eq ':';

     print "$c => $b\n";

     print $fp pack 'C', $b;
   }
   print $fp pack 'x';
} 

close $fp;


__DATA__
  _______                        __ __            
 |       .----.---.-.--.--.-----|  |  .-----.----.
 |.|   | |   _|  _  |  |  |  -__|  |  |  -__|   _|
 `-|.  |-|__| |___._|\___/|_____|__|__|_____|__|  
   |:  |                                          
   |::.|                                          
   `---'                                          
  _______               __                        
 |       .----.---.-.--|  .-----.----.            
 |.|   | |   _|  _  |  _  |  -__|   _|            
 `-|.  |-|__| |___._|_____|_____|__|              
   |:  |                          0.63            
   |::.|                                          
   `---'                                          