/* 	Computes the crc64 cyclic redundancy code
	for "string" and returns it as an
	unsigned long long (64 bits).  Of course,
	a null character in "string" ('\0') will
	end the data and will not be included in
	the CRC returned value. */

unsigned long long crc64(char* string);


