/*!
 * @file derpcl/image.h
 * Definiton of the derpcl image class.
 *
 * @author Rahul A. G.
 */

#ifndef __DERPCL_IMAGE_H__
#define __DERPCL_IMAGE_H__

#include "common.h"
#include "env.h"

using namespace std;

namespace derpcl {

	typedef struct {
		union { size_t width; size_t x; };
		union { size_t height; size_t y; };
	} dim_t;

	class Image {

	public:

		Image(Environment& env, MemoryType m_type, Channels channels, PixelFormat pix_fmt, dim_t dimensions) throw(string);
		~Image();

		Environment const& env() const;
		MemoryType const& type() const;
		Channels const& channels() const;
		PixelFormat const& format() const;
		dim_t const& dimensions() const;
		cl_mem const& block();
		size_t size() const;
		void* data();

		void map(MapMode map_mode);
		void unmap();

		cl_event queueRead(void* destination);
		cl_event queueWrite(void* source);

		Image const& operator=(Image const& other) = delete;

	private:

		Environment& _env;
		MemoryType _m_type;
		Channels _channels;
		PixelFormat _pix_fmt;
		dim_t _dimensions;

		cl_image_format _format;
		cl_image_desc _desc;
		size_t _origin[3];
		size_t _endpoint[3];

		cl_mem _block;
		size_t _size;
		void* _data;

	};

}

#endif
