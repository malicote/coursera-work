from copy import deepcopy
from random import randrange

# Swaps elements i and j in a
def swap(a, i, j):
  temp = a[i]
  a[i] = a[j]
  a[j] = temp


# Partition array assuming pivot is 
# at position p and last element is q
#
# Returns location of pivot in array.
def partitionArray(a, p, q):
  j = p+1
  pivot = a[p]
  for i in xrange(p+1, q+1):
    if a[i] <= pivot:
      swap(a, i, j)
      j += 1
  swap(a, p, j-1)
  return j-1

'''Choose pivot using fpr a[p..q] with a given method:
    1: Just use first element
    2: Use the last element
    3: Find median element
  Returns pivot index
'''
def choosePivot(a, p, q, pivot_method):
  if pivot_method == 1:
    return p
  if pivot_method == 2:
    return q
  if pivot_method == 3:
    middle = int((p + q)/2)
    pivots = [ (a[p], p), (a[q], q), (a[middle], middle) ]
    pivots.sort()
    median, index = pivots[1]
    return index

# Quick sort.  Uses method pivot_method for choosing
# the pivot.  Returns number of comparisons
# made.
PIVOT_METHOD = 1
def quickSort(a, pivot_method):
  global PIVOT_METHOD 
  PIVOT_METHOD = pivot_method
  return quickSort_recursive(a, 0, len(a) - 1)


def quickSort_recursive(a, p, q):
  comparisons = 0

  if (p >= q):
    return comparisons

  pivot_idx = choosePivot(a, p, q, PIVOT_METHOD)
  swap(a, pivot_idx, p)
  pivot_idx = partitionArray(a, p, q)
  comparisons += quickSort_recursive(a, p, pivot_idx - 1)
  comparisons += quickSort_recursive(a, pivot_idx + 1, q)
  comparisons += q - p
  return comparisons

def isSorted(a):
  for i in xrange(1, len(a)):
    if a[i] < a[i-1]:
      return False
  return True

def sortTests():
  number_of_tests = 100
  test_failed = 0;
  for test in xrange(0, number_of_tests):
    p = []

    length = randrange(0, 2500)

    for i in xrange(1,length):
      p.append(randrange(1,100))

    orig = deepcopy(p)
    quickSort(p, 3)
    if not isSorted(p):
      print "Test failed:"
      print orig
      print p
      test_failed += 1
  
  print "Total tests failed: %s" % test_failed


# Reads file pointed to by filename and returns array of ints
def getData(filename):
  with open(filename) as f:
    numbers = [[int(x) for x in line.split()] for line in f]
  return numbers

##################################################
numbers = getData("numbers.txt")
sortTests()
print quickSort(numbers, 3)
print isSorted(numbers)

