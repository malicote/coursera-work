def swap(a, i, j):
  temp = a[i]
  a[i] = a[j]
  a[j] = temp

# Assume pivote at p
def partitionArray(a, p, q):
  pivote = a[p]
  j = p+1
  for i in xrange(p+1, q+1):
    if a[i] <= pivot:
      swap(a, i, j)
      j += 1
  swap(a, p, j-1)
  return j-1

def findNthSmallest_recursive(a, p, q, n):
  
  if p >= q:
    return p 

  idx = findPivot(a, p, q)
  swap(a, idx, 0)
  idx = partitionArray(a, p, q)
  if n < idx:
    return findNthSmallest_recursive(a, 0, idx - 1, n)
  elif n > idx:
    return findNthSmallest_recursive(a, idx + 1, q, n - idx)
  else:
    return idx


def findNthSmallest(a, n):
  return findNthSmallest_recursive(a, 0, len(a) - 1, n)


