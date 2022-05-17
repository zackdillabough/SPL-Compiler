# Simple Programming Language (SPL) Compiler
## Description
This project is a compiler for a programming language capable of: 

1. Integer-type Variables
2. Basic Arithmetic (addition, subtraction, multiplication, integer division)
3. Boolean Logic Operators (less-than, greater-than, not)
4. Conditional Statements (if, switch)
5. Iterative Statements (while, for)
6. Outputting variable values to stdout

## Usage
This compiler takes programs written in a text file as input via redirection, and provides any output to stdout.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```$ ./a.out < program.txt```

A program written in the compiler-specific language is composed of three sections (in order):

1. A Variable Declaration Section
2. A Body Section
3. An Input Section

## The Variable Declaration Section
Variables are declared at the top of a file (before the body section) with the following syntax:

**form:** ```a, b;```
      
## The Body Section
This section is enclosed in curly-brackets.

The body section can consist of the following statements:

---

### 1.) Variable Initialization

&nbsp;&nbsp;&nbsp;&nbsp;**form 1 - initialization from input section:** ```input <variable name>;```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;note: inputs are assigned to variables in the order in which they are provided in the inputs section.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```input G;```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;G will equal the int-value x if the input section consists of "```x, w```" (where ```x``` is an integer, and ```w``` is a potentially empty list of integers &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;delimited by commas) at execution-time.

&nbsp;&nbsp;&nbsp;&nbsp;**form 2 - explicit initialization:** ```input <integer literal>;```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```G = 5;```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;G will equal 5 at execution-time.

---

### 2.) Basic Arithmetic Operations

&nbsp;&nbsp;&nbsp;&nbsp;**form:** ```<var> = <var or literal> <operator> <var or literal>;```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```G = 1 + 2;  \\ G now holds the value of 3```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```sum = sum - 1;  \\ sum has been decremented by 1```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```growth = original * 2;  \\ growth now holds double the amount of original```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```x = 7 / 2;  \\ x now holds the value of 3```

---

### 3.) Boolean Logic and Conditional/Looping statements

&nbsp;&nbsp;&nbsp;&nbsp;**if form:** ```IF <variable or name> <boolean operator> <variable or name> { <statements> }```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```IF G < 0 { ... }  \\ enters IF block if the value in G is negative```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```IF answerToUniverse <> 42 { ... }  \\ enters IF block if answerToUniverse does not equal 42```

&nbsp;&nbsp;&nbsp;&nbsp;**while form:** ```WHILE <variable or name> <boolean operator> <variable or name> { <statements> }```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```WHILE i > 0 { ... }  \\ enters and loops through WHILE block while the value in i is greater than zero```

&nbsp;&nbsp;&nbsp;&nbsp;**switch form:** ```SWITCH <variable or name> { CASE x: { ... } CASE y: { ... } ... }  \\ where x and y are integer literals```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) 

```
SWITCH a {
  CASE 1: { output a; }
  CASE 2: { a = a - 50; }
  CASE 3: { output b; }
}
```

---

### 4.) Output Statements

&nbsp;&nbsp;&nbsp;&nbsp;**form:** ```output <variable>;```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```output a;  \\ outputs the value stored in a```

## The User Input Section
This section is a line at the bottom of the program that consists of a non-empty list of integers.
There must be a sufficient number of integers specified in this line for all INPUT statements (There cannot be more INPUT statements than there are user inputs).
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ex.) ```-3 1 4 1 5 9```

### Simple Examples
Program_1.txt
```
i, j;
{
	input i;
  	i = 42 ;
  	j = i + 1; 
	output i;
	output j;
}
1 2 
```
Terminal output
```
$ ./a.out < Program_1.txt
42 43
```  
Program_2.txt
```
j, i, minusfour, k ;

{

input i;
input j;
minusfour = 0-4;

WHILE i > 0 {
	j = 3;	
	IF j > 1 {	
		WHILE j > 0 {
			k = i*j;
			output k;
			j = j-1;
		}
	}
	IF j > 0 {
		WHILE j > 0 {
			k = i*j;
			output k;
			j = j-1;
		}
	}
	i = i-1;
}

input minusfour;

WHILE i > minusfour {
	output i;
	i = i - 1;
}

i = i+1;
j = j+1;
output i;
output j;

}
4 3 4 4 5 2 6
```
Terminal Output
```
$ ./a.out < Program_1.txt
12 8 4 9 6 3 6 4 2 3 2 1 1 1
```
