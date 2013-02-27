#include "image.h"

using namespace derpcl;

Image::Image(Environment& env, MemoryType m_type, Channels channels, PixelFormat pix_format, dim_t dimensions) throw(string) : _env(env), _m_type(m_type), _channels(channels), _pix_fmt(pix_format), _dimensions(dimensions) {
	
	_format.image_channel_order = static_cast<cl_channel_order>(channels);
	_format.image_channel_data_type = static_cast<cl_channel_type>(pix_format);

	_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
	_desc.image_width = _dimensions.width;
	_desc.image_height = _dimensions.height;
	_desc.image_depth = 1;
	_desc.image_array_size = 1;
	_desc.image_row_pitch = 0;
	_desc.num_mip_levels = 0;
	_desc.num_samples = 0;
	_desc.buffer = nullptr;

	_origin[0] = _origin[1] = _origin[2] = 0;
	_endpoint[0] = _dimensions.width;
	_endpoint[1] = _dimensions.height;
	_endpoint[2] = 1;

	cl_int error;

	_block = clCreateImage(_env.context(), static_cast<cl_mem_flags>(_m_type), &_format, &_desc, NULL, &error);
	if (error != CL_SUCCESS)
	{
		throw string("Error creating clImage: ") + cl_err_to_string(error);
	}

	size_t row_pitch;
	error = clGetImageInfo(_block, CL_IMAGE_ROW_PITCH, sizeof(size_t), &row_pitch, NULL);
	if (error != CL_SUCCESS)
	{
		throw string("Error getting clImage info: ") + cl_err_to_string(error);
	}

	_size = _dimensions.height * row_pitch;

}

Image::~Image() {
	 clReleaseMemObject(_block);
}

Environment const& Image::env() const {
 return _env;
}

MemoryType const& Image::type() const {
 return _m_type;
}

Channels const& Image::channels() const {
 return _channels;
}

PixelFormat const& Image::format() const {
 return _pix_fmt;
}

dim_t const& Image::dimensions() const {
 return _dimensions;
}

cl_mem Image::block() {
 return _block;
}

cl_mem* Image::block_ptr() {
 return &_block;
}

size_t Image::size() const {
 return _size;
}

void* Image::data() {
 return _data;
}

void Image::map(MapMode map_mode) {
	
	cl_int error;
	size_t row_pitch;

	_data = clEnqueueMapImage(_env.queue(), _block, CL_TRUE, static_cast<cl_map_flags>(_m_type), _origin, _endpoint, &row_pitch, NULL, 0, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		throw string("Error mapping image: ") + cl_err_to_string(error);
	}

}

void Image::unmap() {
	clEnqueueUnmapMemObject(_env.queue(), _block, _data, 0, NULL, NULL);
}

cl_event Image::queueRead(void* destination) {
	cl_event ev;
	clEnqueueReadImage(_env.queue(), _block, CL_FALSE, _origin, _endpoint, 0, 0, destination, 0, NULL, &ev);
	return ev;
}

cl_event Image::queueWrite(void* source) {
	cl_event ev;
	clEnqueueWriteImage(_env.queue(), _block, CL_FALSE, _origin, _endpoint, 0, 0, source, 0, NULL, &ev);
	return ev;
}
