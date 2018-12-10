# MAG for Circullar Pattern Matching (MAGC)

This source code was written for research purpose and has a minimal error checking. The code may be not very readable and comments may not be adequate. There is no warranty, your use of this code is at your own risk.

The algorithm is searching the circullar patterns in a given text file. It is a variant of MAG algorithm [SGF2014] (https://github.com/rsusik/mag).

## Requirements
* C++ compiler compatible 
* Unix based 64-bit OS (compilation works also in Cygwin)
* Python 3 (for [testing](#testing))
* [Docker](#docker) (optionally)

## Compilation
To compile the code run below line commands:
```shell
git clone https://github.com/rsusik/magc magc
cd magc
make all
```

## Running
To execute searching following command needs to be executed:

```shell 
./[alg] [patterns] [m] [set] [u] [k] [q] [sig]
```
where:
* alg      - one of compiled executable (e.g. *mag*)
* patterns - file with patterns (each *m* bytes long - no separator)
* set      - text file (e.g. *english.200MB*)
* u        - FAOSO parameter [FG09] (e.g. 4)
* k        - FAOSO parameter [FG09] (e.g. 2)
* q        - *q*-gram size (e.g. 3)
* sig      - alph size (used for mapping, e.g. 5)

**Note:** For more details about the parameters please refer to [SGF2014] (http://www.stringology.org/event/2014/p07.html)

Example:

```shell
./cpm_mag_dna_l2 ./patterns.r1/patterns.english.200MB.8.bin 8 ./sets/english.200MB 8 1 2 5
```

## Testing

### Datasets
The datasets that were used for testing can be downloaded from Pizza\&Chilli Corpus (http://pizzachili.dcc.uchile.cl/texts.html).
If below python scripts are used for testing then place the downloaded data files in `sets` folder.
**Note:** Mentioned corpuses may be downloaded automatically by testing script `magc_test.py`.

### Scripts
* `generate_patterns.py` - generates patterns (see Testing section)
* `magc_test.py`         - tests the algorithms (see Testing section)

To make the testing with multiple parameters easier the `magc_test.py` script can be used. 

```
usage: magc_test.py [-h] [-r R] [-a A] [-c C] [-m M] [-u U] [-k K] [-q Q]
                    [-s S]

optional arguments:
  -h, --help           show this help message and exit
  -r R, --npatterns R  number of patterns
  -a A, --algorithm A  algorithm[s] to be tested
  -c C, --corpus C     corpus
  -m M, --length M     pattern length[s] (e.g. 8,16,32)
  -u U, --faosou U     FAOSO parameter U
  -k K, --faosok K     FAOSO parameter k
  -q Q, --q-gram Q     q-gram size
  -s S, --sigma S      dest. alph. size

Example: python3 magc_test.py -a cpm_mag_dna_opt_l2 -c dna.100MB -m
32,64 -q 4,6,8 -u 4,8 -k 1,2
```

**Note:** Please replace *pattern_loc*, *set_loc* and *alg_loc* variables in the file is needed.

To execute test for:
- english.200MB dataset,
- of length 8, 16, 32, 64,
- with u = 8,
- k = [1, 2],
- q = [2, 3, 4, 5, 6, 7, 8]
- and sigma = 5

the following command needs to be run:

```shell
python3 magc_test.py -a cpm_mag_dna_l2 -c english.200MB -m 8,16,32,64 -u 8 -k 1,2 -q 2,3,4,6,8 -s 5
```

**Note:** If the set of parameters is incorrect (e.g. *q > m*) the errors will produced in output (which can be ignored). The algorithm was not tested for more than *r*=1 circullar pattern.

**Generate patterns**

There are example patterns included in `patterns.r1` folder but if there is need to generate a new ones or generate patterns for different datasets the script `generate_patterns.py` may be used.

Example:
```shell
$ python generatePatterns.py -c english.200MB -m 8,16,32,64
```

The above command generates patterns of length *m*={8, 16, 32, 64} for english.200MB dataset.

**Note:** Please make sure the folders *patterns.r[r]* (where [r] is the number of patterns) exist (e.g. ./patterns.r100).


## Docker

The simplest way you can test the algorithm is by using docker. All you need to do is to:

* Pull or build the image:

	* Pull from repository steps:
	```
	docker pull rsusik/magc
	docker tag docker.io/rsusik/magc magc
	```

	* Build steps:
		* Clone the git repository:
		```shell
		git clone https://github.com/rsusik/magc magc
		cd magc
		```
		* Build the image:
		```shell
		docker build -t magc .
		```

* Build the image:
```shell
docker build -t magc .
```

* Run container:
```shell
docker run --rm magc
```

* Additionally you may add parameters (as mentioned in Testing section):
```shell
docker run --rm magc -c english.100MB -m 32
```

**Note:** Docker may have performance impact on algorithm so it is recomended to 
test the algorithms natively on the same machine.

## Citing
	@phdthesis{Susik2018,
		title    = {Techniki kompresji i równoległości bitowej w wybranych problemach wyszukiwania wzorców w tekście},
		school   = {Politechnika Łódzka},
		author   = {Robert Susik},
		year     = {2018},
		type     = {{PhD} dissertation}
	}

## References
[FG09] K. Fredriksson, S. Grabowski.
*Average-optimal string matching. Journal of Discrete*,
pages 579–594. Algorithms 7(4), 2009.

[SGF2014] R. Susik, Sz. Grabowski, and K. Fredriksson.
*Multiple pattern matching revisited.*
In Proceedings of the Prague Stringology Conference, pages 59–70, Czech Technical University in Prague, Czech Republic, 2014


## Authors
* Robert Susik
