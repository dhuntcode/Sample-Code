/*  Buffer for sampling
    written by Dean Hunt   */

#include <iostream>
#include "mbed.h"
using namespace std;

//Template Buffer class that can 

template <class T>
class Buffer {
    
    private:
    int _getpos;    //Where to get oldest sample
    int _size;  //Max size of the buffer
    int _putpos; //Where to put newest sample
    int _freespace; //How much space is left in the buffer
    T *_data; // Pointer to array of type T
    bool _full; // Is the buffer full?
    bool _empty; //Is the buffer empty?
    Mutex mutex; // Lock for critical sections

    public:
    Buffer(int size): _getpos(), _putpos(), _size(size), _data(NULL){ //make a new buffer with a certain size.  Set values of position to 0 and data to NULL to avoid sampling rubbish
        _data = new T[size];
        _freespace=_size;
        _getpos = 0;
        _putpos = 0;
    }

    ~Buffer (){                   // destructor to free up memory once the Buffer has been used.
        delete [] _data;
    }

    int size() {                 // basic method to get the size of buffer
        return _size;
    }

    int samplesInBuffer()       //Shows how many samples are valid date inside the buffer
    {
        return (_size - _freespace);
    }
    int startPoint()            //Shows the position at which the next sample is to be taken
    {
        return _getpos;
    }

    void add(T sample) {    // add samples, Thread safe with mutex.

        if(_freespace < 0)  //if freespace shows 0, set the full flag high
        {
            _full = true;
        }
        else{
            _full = false;  //otherwise, it's not full
        }
        if (_full == false){
        mutex.lock();
        _data[_putpos] = sample;    //Store data in available space
        _putpos++;                  //Increment that space
        _freespace--;               //Free space reduced
        mutex.unlock();
        if(_putpos > _size)         //Wrap around once you're at the end of the buffer. This is what makes it 'circular'
            {
                 _putpos=0;
            }
        }
    }       

    T &peek(int pos)            //Return data at a point in the array without removing it or moving the pointer. Peek instead of pull
    {
        return _data[pos];
    }

    T get() {       // get sample in a posistion of the buffer.    
            mutex.lock();  
            int tempPos = _getpos; //Temporary store position, as it's going to change   
            _getpos++;          //Increment the position now, once data is returned, there's no going back
            _freespace++;       //Show some space has freed up
            if(_getpos > _size) //Wrap around if at the end
            {
                _getpos=0;
            }   
            mutex.unlock();
            return _data[tempPos];   // Copying over refrence to save overhead.
    }    

    int status(){

        if(_freespace == 10){       //Once the buffer is 10 samples from full, let the program know it's time to FLUSH
            return 2;               //Even at fastest sample rate, 0.1S, this gives us a full second extra buffering while it's writing
        }
        else if(_full == true){     //Return status 3 if full, handler will throw an error
            _full = false;
            return 3;
        }
        else if (_empty == true){   //Status 4 if empty, this doesn't tend to happen, but isn't detrimental
            _empty = false;
            return 4;
        }
        else{
            return 1;
        }
    }
};