# TridentineLectionary
A Latin Mass liturgical calendar generator

## Build
The program can be built with Visual Studio 2017 on Windows or with gcc on Linux using (in the source directory) ``` cmake ./ ``` followed by the ``` make ``` command. A pre-built Windows binary is available in the Debug folder (in the same folder as this Readme file).

## Examples
[Liturgical Year 2018-2019](Debug/2019.csv)

[Liturgical Year 2019-2020](Debug/2020.csv)

## Documentation for en-1962.yml file
This file contains the text in English for Moveable feasts, as well a a few configuration options for enabling/disabling the inclusion of optional Votive Masses.

These options are at the start of the file, under the key en.tridentine.config:

```yml
en:
  tridentine:
    config:
      celebrate_christ_priest: false
      celebrate_sacred_heart: false
      celebrate_immaculate_heart: false
      celebrate_immaculate_heart_in_eastertide: true
      celebrate_sat_of_our_lady: true
      celebrate_mass_for_the_dead: true
      celebrate_rorate_coeli_in_advent_on_day: '1'
```

``` en.tridentine.config.celebrate_mass_for_the_dead: ``` Enables the Daily Mass for the Dead in the week following All Saints' day (1 November)

``` en.tridentine.config.celebrate_rorate_coeli_in_advent_on_day: ``` Celebrates the optional Mass Roráte cœli désuper on the specified day of Advent, if it is not a Sunday in Advent or superseded by a feast with higher priority.

## Documentation for en-1962.txt file
This file is a formatted text file containing details of the Fixed feasts of the liturgical year.

### Example - December in the en-1962.txt file
```
= 12
2 3 R : St. Bibiana Virgin and Martyr, III class.
3 3 : St. Francis Xavier Confessor, III class.
4 3 : St. Peter Chrysologus Bishop, Confessor, and Doctor of the Church, III class
4 c : Com. of St. Barbara Virgin and Martyr.
5 c : Commemoration of St. Sabbas Abbot, Comm.
6 3 : St. Nicholas Bishop and Confessor, III class.
7 3 : St. Ambrose Bishop, Confessor, and Doctor of the Church, III class.
8 1 o i : On the Immaculate Conception of the Blessed Virgin Mary, I class.
10 c : Commemoration of St. Melchiades Pope and Martyr, Comm.
11 3 : St. Damasus I Pope and Confessor, III class.
13 3 R : St. Lucy Virgin and Martyr, III class.
16 3 R : St. Eusebius Bishop and Martyr, III class.
21 2 : St. Thomas Apostle, II class.
24 1 f : Vigil of the Nativity of Our Lord
25_0 1 o f : The Nativity of Our Lord, First Mass at Midnight
25_6 1 o f : The Nativity of Our Lord, Second Mass at Dawn
25_6 c 1 : Commemoration of St. Anastasia Martyr.
25_12 1 o f : The Nativity of Our Lord, Third Mass during the day
25 1 o f : The Nativity of Our Lord
26 2 R : II day within the octave of the Nativity of the Lord: St. Stephen Protomartyr, II class.
27 2 : III day within the octave of the Nativity of the Lord: St. John Apostle and Evangelist, II class.
28 2 R : IV day within the octave of the Nativity of the Lord: The Holy Innocents, II class.
29 2 : Of the V day within the octave of the Nativity of the Lord, II class
29 c : Commemoration of St. Thomas Bishop and Martyr.
30 2 : Of the VI day within the Octave of the Nativity, II class.
31 2 : Of the VII day within the Octave of the Nativity, II class
31 c : Commemoration of St. Sylvester I Pope and Confessor.
```

The file contains entries for all days of the year for which a Feast and/or Commemoration occurs (sometimes for both if there is both a Commemoration and a Feast on a single day).

#### Format of an entry
``` = 12 ``` - Month of year

Each Mass/Commemoration line is of the form ```<day of month>[_hour of day] <flags> : <Text of Feast or Commemoration> ```

#### Flags:

``` 1 ``` ``` 2 ``` ``` 3 ```: Class of Feast/Commemoration (Class 4 is assumed if not specified)

``` o ```: Flag for Holy Day of Obligation

``` f ```: Flag for Feast of the Lord

``` i ```: Flag for Feast of the Immaculate Conception. This is unique in that it alone, other than Feasts of the Lord, can override a Sunday in Advent

``` c ```: Flag for Commemoration

``` R ```: Flag for Red liturgical colour/vestments

``` G ```: Flag for Green liturgical colour/vestments

``` V ```: Flag for Violet liturgical colour/vestments

``` W ```: Flag for White liturgical colour/vestments

``` P ```: Flag for Rose liturgical colour/vestments

The Liturgical Colour defaults to White if there is a Feast (not a Commemoration) for the day in question, so it does not need to be specified in these cases.

In practice, only Red (``` R ```) needs to be specified, since Liturgical Colours for Feasts are either Red, White, or the colour of the liturgical season. 

If there is no fixed Feast for the day in question, or if there is only a Commemoration, the liturgical colour used will be the colour of the liturgical season.

The file also enables multiple Masses to be celebrated on the same day by allowing entries to be repeated along with a number denoting the hour of the day (eg. for Christmas Day), but the feature is not used in this program (the program was designed to be part of the Lectionary project (https://github.com/plishman/Catholic-Lectionary-on-ESP8266), to enable the Tridentine Mass to be supported).
