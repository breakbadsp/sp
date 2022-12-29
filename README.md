# sp
this is a data structures liabrary
#1 Added Async Queue:
- AsyncQueue can be used for communication between two threads
- Added sample example producer and consumer to demonstrate the use of AsyncQueue
- Producer can continuresly enqueu data into AsyncQeueue
- Consumer can continuesly deque the data from AsyncQueue

AsyncQueue has below APIs:
1. Enqueue(T p_data):
  - This api can be used to queue a single element into AsyncQueue
  - This will notify one consumer imediately
  
2. T Dequeue():
  - Can be user to dequeu one element from the AsyncQueue
  - It will be a blocking call until there is data in AsyncQueue
  
3. EnqueueBulk():
  - Will notify consumer only after enqueing dequeu_count_ number of elements.

4. DequeueBulk():
  - Will dequeu only when >= dequeue_count_ number of elements are avaialble.
  - Will notify one thread once all data is read.(will be used by #5)

5. BlockingEnqueueBulk():
  - Like #3 but will block until AsyncQueue is not read completely.
