//! \file string+.h \brief implements functions useful on strings, like split(string,delimeter)

#pragma once

#include<string>
#include<vector>
#include<cstdlib>
//! this is just a handy function that splits a string along a given character into an array of strings
/*! \param split_me is the string that needs to be smashed into pieces and placed in a vector
 * \param delim is the character along which the function does the smashing. There can be no spaces after
 *the meaningful information, or it will likely return a bad vector */
std::vector<std::string> split(std::string split_me, char delim);

void print_tokens(std::ostream& outs, const std::vector<std::string>& print_me); 


//! this function trims the first argument's size by the length of the second argument.
/* this function is used to ensure that the character array field::temp_input is the same size as the one expected
 *in input_maker's fortran string struct
 * \param base_string the string who will lose size(string_remove) characters
 * \param remove_num integer value that is the n characters to be "lopped off" the end of the string */
void trim(std::string& base_string,unsigned int remove_num);

//! this function loops over the passed in string, and builds a new string that excludes the passed character
/*! \param string_in will be looped over, and the good characters stored in an accumulator string that is assigned to string_in at the end of the functoin
 *\param strip_me is the character that will not be in the "new" string */
void strip_char(std::string& string_in,char strip_me);
