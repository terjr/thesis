#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <iostream>

void decompress(std::istream *stream, boost::iostreams::filtering_istream &filtered);
