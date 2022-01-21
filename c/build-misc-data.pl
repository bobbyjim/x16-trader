
my @missionCodes = qw/
trader   
beagle   
cruiser  
defender 
escort   
freighter
frigate  
unknown  
unknown  
seeker   
safari   
lab      
liner    
survey   
unknown  
corsair  
unknown  
merchant 
scout    
transport
packet   
destroyer
barge    
express  
yacht    
/;

my @title = (
'.#######.########....###...##.....#.#######.##......##......#######.########..',
'....##...##.....#...##.##..##.....#.##......##......##......##......##.....##.',
'....##...##.....#..##...##.##.....#.##......##......##......##......##.....##.',
'....##...########.##.....#.##.....#.######..##......##......######..########..',
'....##...##...##..########..##...##.##......##......##......##......##...##...',
'....##...##....##.##.....#...##.##..##......##......##......##......##....##..',
'....##...##.....#.##.....#....###...#######.#######.#######.#######.##.....##.',
'....  ...  ..... .  ..... ....   ...       .       .       .       .  .....  .',
'.#######.########....###...########.#######.########..........................',
'....##...##.....#...##.##..##.....#.##......##.....##.........................',
'....##...##.....#..##...##.##.....#.##......##.....##.........................',
'....##...########.##.....#.##.....#.######..########..........................',
'....##...##...##..########.##.....#.##......##...##...........................',
'....##...##....##.##.....#.##.....#.##......##....##..........................',
'....##...##.....#.##.....#.########.#######.##.....##.........................',
'....  ...  ..... .  ..... .        .       .  .....  .........................',
'..............................................................................',
);

my @owner = (
    '0 - Unknown',
    '1 - Unknown',
    '2 - Unknown',
    '3 - Unknown',
    '4 - Unknown',
    '5 - Unknown',
    '6 - Unknown',
    '7 - Unknown',
    '8 - Unknown',
    '9 - Unknown',
    ': - Unknown',
    '; - Unknown',
    '< - Unknown',
    '= - Unknown',
    '> - Unknown',
    '? - Unknown',
    '@ - Unknown',
    'Aslan',
    'Baraccai Tech.',
    'C - Unknown',
    'Droyne',
    'E - Unknown',
    'F - Unknown',
    'G - Unknown',
    'Humbolt',
    'Imperial',
    'Judges Guild',
    'K - Unknown',
    'Delta',
    'Al Morai',
    'N - Unknown',
    'O - Unknown',
    'Paranoia Press',
    'Q - Unknown',
    'Republic',
    'Sword Worlds',
    'Tukera',
    'Universal',
    'Vargr',
    'W - Unknown',
    'Exotic',
    'DayStar',
    'Zhodani',
);


open my $miscBinary, '>', 'MISC.BIN';

#
#  Requisite two Commodore address nulls
#
print $miscBinary pack 'xx';

#
#  Mission strings begin on 0xA000.
#
#  10 x 25 = 250 bytes.  A thru Y.
#
foreach my $code (@missionCodes)
{
    print $miscBinary sprintf "%-9s", $code;
    print $miscBinary pack 'x';
}

#
#  Pad with 6 dashes.
#
print $miscBinary '------';

#
#  Title begins on 0xA100.
#
foreach my $line (@title)
{
    print $miscBinary sprintf "%79s", $line;
    print $miscBinary pack 'x';
}

#
#  Owner strings begin at $A652.
#
foreach my $owner (@owner)
{
    print $miscBinary sprintf "%-15s", $owner;
    print $miscBinary pack 'x';
}

close $miscBinary;
