This is my second assignment for the honors contract for ECE 264.

This program takes text files and compresses them using Huffman trees. It assigns variable length binary codes for each input character in the text file based on how often they occur. In addition, the program decompresses the file after to prove that all the data is retained, ensuring that the data compression is "lossless."

The expected files are calculated manually by counting the number of characters and their frequencies. 

For example, if the text being read is "Stressed-desserts", then the size of a file with this text would be 17 bytes, because there are 17 character each assigned one byte. 

However, using huffman encoding, the same text would onyl be 5.5 bytes: 1*S + 1*- + 2*d + 4*e + 2*r + 5*s + 2*t = 1*4 + 1*4 + 2*3 + 4*2 + 2*3 + 5*2 + 2*3 =  44 bits = 44/8 bytes = 5.5 bytes.

