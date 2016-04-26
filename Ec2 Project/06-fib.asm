---------------------------------------
-- Computes the nth Fibonacci number --
---------------------------------------
-- Input an integer, n
IN A
-- Quit if zero or negative
JZ     00011 -- jump to HALT
JNNEG  00100
HALT
-- Store into count (address 4 = 00100)
STORE A, 11100 -- count
-- Decrement count (address 5 = 00101)
LOAD  A, 11100 -- count
SUB   A, 11000 -- Subtract 1
STORE A, 11100
-- If count == 0, done
LOAD  A, 11100 -- Count
JZ       10011
-- Add f(n-2) + f(n-1), store in f(n)
LOAD  A, 11001 -- f(n-2)
ADD   A, 11010 -- f(n-1)
STORE A, 11011 -- f(n)
-- Shift values
LOAD  A, 11010
STORE A, 11001
LOAD  A, 11011
STORE A, 11010
LOAD  A, 11000  -- Set A = 1
JNNEG    00101  -- to unconditionally jump
-- Display result (address 19 = 10011)
LOAD A,  11011
HALT
-- Padding (so the data starts at a nice address)
HALT
HALT
HALT
-- Data after this (addresses 24+)
LOAD A, 00001    -- Address 11000: Value 1 is stored here (encodes to the value 1)
LOAD A, 00000    -- Address 11001: f(n-2) stored here (encodes to 0)
LOAD A, 00001    -- Address 11010: f(n-1) stored here (encodes to 1)
LOAD A, 00001    -- Address 11011: f(n) stored here (encodes to 1)
LOAD A, 00000    -- Address 11100: count stored here
