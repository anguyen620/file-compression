Usage: rle <input file> <output file> <compression length> <mode>
  
-Compression length: the length of the pattern in which to compress/decompress if there are adjacent identical patterns.
    For Example: AAAABC will compress to 4ABC with a compression length of 1. With a compression length of 2, it will become 2AABC.
-Mode: 0 for compression, 1 for decompression.
