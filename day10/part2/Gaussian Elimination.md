
```
[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
```

columns = buttons
Set 1 if the button affects the joltage counter

| b0  | b1  | b2  | b3  | b4  | b5  | t   |
| --- | --- | --- | --- | --- | --- | --- |
|     |     |     |     | 1   | 1   | 3   |
|     | 1   |     |     |     | 1   | 5   |
| 1   | 1   |     | 1   |     |     | 7   |
|     |     | 1   | 1   | 1   |     | 4   |
Get to the row echelon form, all values below the diagonal need to be 0
In this case just by swapping rows

| b0  | b1  | b2  | b3  | b4  | b5  | t   |
| --- | --- | --- | --- | --- | --- | --- |
| 1   | 1   |     | 1   |     |     | 7   |
|     | 1   |     |     |     | 1   | 5   |
|     |     | 1   | 1   | 1   |     | 4   |
|     |     |     |     | 1   | 1   | 3   |
Equations for each row

```
b0 + b1 + b3 = 7
b1 + b5 = 5
b2 + b3 + b4 = 4
b4 + b5 = 3
```

Write equation for each pivot (first non 0 in each row)

```
b4 = 3 - b5

b2 = 4 - b3 - b4
b2 = 4 - b3 - (3 - b5)
b2 = 4 - b3 - 3 + b5
b2 = 1 - b3 + b5

b1 = 5 - b5

b0 = 7 - b1 - b3
b0 = 7 - (5 - b5) - b3
b0 = 7 - 5 + b5 - b3
b0 = 2 - b3 + b5
```

Constraints

```
b0 = 2 - b3 + b5
b1 = 5 - b5
b2 = 1 - b3 + b5
b3 = free >= 0
b4 = 3 - b5
b5 = free >= 0

2 - b3 + b5 >= 0
b5 >= b3 - 2

5 - b5 >= 0
b5 <= 5

1 - b3 + b5 >= 0
b5 >= b3 - 1

3 - b5 >= 0
b5 <= 3

b5 >= 0
b3 >= 0
```

Combine constraints
```
b5 >= b3 - 1
b5 >= b3 - 2
b5 >= max(b3 - 1, b3 - 2)
b5 >= b3 - 1

b5 <= 5
b5 <= 3
b3 >= 0
b5 <= min(5, 3)
b5 <= 3
```

Total presses
```
b0 + b1 + b2 + b3 + b4 + b5
= (2 - b3 + b5) + (5 - b5) + (1 - b3 + b5) + b3 + (3 - b5) + b5
= 2 - b3 + b5 + 5 - b5 + 1 - b3 + b5 + b3 + 3 - b5 + b5
= 2 + 5 + 1 + 3 + (-b3 - b3 + b3) + (b5 - b5 + b5 - b5 + b5)
= 11 - b3 + b5
```

Minimize
```
b3 >= 0
b5 <= 3
b5 >= b3 - 1
b5 >= 0


sum = 11 - b3 + b5
minimize -b3 + b5

b3 - 1 <= b5
-1 <= - b3 + b5

min(b5 - b3) = -1

sum = 11 - 1
sum = 10

```