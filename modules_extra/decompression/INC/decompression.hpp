#pragma once

#include <srl.hpp>

namespace SRL::Decompression
{    
	/**
	 * @brief RLE Decompression
	 * Very basic implentation
	 * @param Compressed data input
	 * @param Decompressed data output
	 * @param Decompressed length
	 */ 
	class Rle final
	{
	    public:
        static inline void Decompress(const uint8_t* input, uint8_t* output, size_t decompressedSize)
        {
            size_t in  = 0;
            size_t out = 0;

            while (out < decompressedSize)
            {
                uint8_t run   = input[in++];
                uint8_t value = input[in++];

                for (uint8_t j = 0; j < run; j++)
                {
                    output[out++] = value;
                }
            }
        }
    };
    
    class Lzss final
    {
    public:
        /**
         * @brief LZSS Header
         * - String to detect LZSS compressed file
         * - Version Number
         * - Original File Type
         * - Original File Size
         * - Reserved / Future
         */
        struct Header
        {
            uint32_t magic;        // "LZSS"
            uint32_t version;
            uint32_t fileType;
            uint32_t originalSize;
            uint32_t reserved;
        };
        
        /**
         * @brief Compressed filetypes
         */
        enum FileTypes : uint32_t
        {
            Lz  = 0x4C5A5353, // `LZSS`
            Pcm = 0x2E50434D, // '.PCM'
            Snd = 0x2E534E44, // '.SND' (packed .PCM sound format)
            Tga = 0x2E544741, // `.TGA`
            Tm  = 0x2E544D00, // `.TM`  (packed VDP1 image format)
            Bin = 0x2E42494E  // `.BIN`
        };
        
        /**
         * @brief LZSS Decompression
         * Format:
         *  - Flags byte: 8 bits, MSB first
         *    1 = literal byte
         *    0 = (offset,length) pair (2 bytes)
         *  - Offset: 12 bits (1..4095)
         *  - Length: 4 bits + 3 (3..18)
         * @param Compressed data input
         * @param Decompressed data output
         * @param Decompressed length
         */
        static inline void Decompress(const uint8_t* input, uint8_t* output, size_t decompressedSize)
        {
            size_t inp  = 0;
            size_t outp = 0;

            uint8_t flags = 0;
            uint8_t mask  = 0;

            while (outp < decompressedSize)
            {
                if (mask == 0)
                {
                    flags = input[inp++];
                    mask  = 0x80; // MSB first
                }

                if (flags & mask)
                {
                    // literal
                    output[outp++] = input[inp++];
                }
                else
                {
                    // match
                    uint16_t pair = ((uint16_t)input[inp] << 8) | input[inp + 1];
                    inp += 2;

                    uint16_t encoded_offset = (pair >> 4) & 0x0FFF;
                    uint16_t length         = (pair & 0x000F) + 3;

                    uint16_t distance = encoded_offset + 1;
                    size_t src = outp - distance;

                    // optional safety check (can remove for speed)
                    // if (distance == 0 || src >= outp) return;

                    for (uint16_t i = 0; i < length; i++)
                    {
                        output[outp++] = output[src + i];
                    }
                }

                mask >>= 1;
            }
        }
    };
}
