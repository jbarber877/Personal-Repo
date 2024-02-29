class Stack:
    def __init__(self):
        '''
        Initialize an empty list to store elements
        '''
        self.items = []

    def push(self, item):
        '''
        Push an element to the top of the stack
        '''
        self.items.append(item)

    def pop(self):
        '''
        Remove the top element from the stackand return it.
        If the stack is empty, raise an error.
        '''
        if not self.is_empty():
            return self.items.pop()
        else:
            raise IndexError("Stack is empty - pop from empty stack")
            
    def is_empty(self):
        '''
        Check if the stack is empty
        '''
        return len(self.items) == 0
    
    def peek(self):
        '''
        Return the top element of the stack without removing it.
        If the stack is empty, return None
        '''
        if not self.is_empty():
            return self.items[-1]
        else:
            return None
            
    def size(self):
        '''
        Get the size of the stack / number of elements in the list
        '''
        return len(self.items)
    
    def empty(self):
        '''
        Delete all items in the stack
        '''
        self.items = []
        
    def show(self):
        '''
        Print all the items in the stack
        '''
        for item in reversed(self.items):
            print(item, end=" ")
        print()



class Queue:
    def __init__(self):
        '''
        Initialize an empty list to store elements
        '''
        self.items = []

    def push(self, item):
        '''
        Add an element to the end of the queue
        '''
        self.items.append(item)

    def pop(self):
        '''
        Remove the top element from the stackand return it.
        If the stack is empty, raise an error.
        '''
        if not self.is_empty():
            return self.items.pop(0)
        else:
            raise IndexError("Queue is empty - pop from empty queue")
            
    def is_empty(self):
        '''
        Check if the stack is empty
        '''
        return len(self.items) == 0
    
    def peek(self):
        '''
        Return the top element of the stack without removing it.
        If the stack is empty, return None
        '''
        if not self.is_empty():
            return self.items[0]
        else:
            return None
            
    def size(self):
        '''
        Get the size of the stack / number of elements in the list
        '''
        return len(self.items)
    
    def empty(self):
        '''
        Delete all items in the stack
        '''
        self.items = []
        
    def show(self):
        '''
        Print all the items in the stack
        '''
        for item in self.items:
            print(item, end=" ")
        print()
