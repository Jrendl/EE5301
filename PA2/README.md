# PA2 #
## Phase 1 ##
### File Descriptions ###
* ``input_parser``: class designed to parse the input file. Currently only parses the node names.
* ``sizer``: class that performs the forward and backward traversal of the tree. 
* ``main``: executes the different portions of the problem and also has the function to create an initial string. This function will most likely be moved to an annealing class for phase 2.

### Known Issues ###
* I forgot about the existence of multi-digit numbers and therefore the program doesn't work with anything more than 0-9. 
    * To fix this, I could make the polish expression an array of chars/strings and instead of indexing into a char array, index into that array.
    * The sizes are already working so this should be a simple fix