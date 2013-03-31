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
	_desc.image_slice_pitch = 0;
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

	error = clGetImageInfo(_block, CL_IMAGE_ROW_PITCH, sizeof(size_t), &_desc.image_row_pitch, NULL);
	if (error != CL_SUCCESS)
	{
		throw string("Error getting clImage info: ") + cl_err_to_string(error);
	}

	_size = _dimensions.height * _desc.image_row_pitch;

}

Image::Image(Image const& other) throw(string) : _env(other._env) {
	*this = other;
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

cl_mem const& Image::block() {
	return _block;
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

	_data = clEnqueueMapImage(_env.queue(), _block, CL_TRUE, static_cast<cl_map_flags>(map_mode), _origin, _endpoint, &row_pitch, NULL, 0, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		throw string("Error mapping image: ") + cl_err_to_string(error);
	}

}

void Image::unmap() {
	cl_event unmap_ev;
	clEnqueueUnmapMemObject(_env.queue(), _block, _data, 0, NULL, &unmap_ev);
	clWaitForEvents(1, &unmap_ev);
}

cl_event Image::queueRead(void* destination) {
	cl_event ev;
	clEnqueueReadImage(_env.txQueue(), _block, CL_FALSE, _origin, _endpoint, 0, 0, destination, 0, NULL, &ev);
	return ev;
}

cl_event Image::queueWrite(void* source) {
	cl_event ev;
	clEnqueueWriteImage(_env.txQueue(), _block, CL_FALSE, _origin, _endpoint, 0, 0, source, 0, NULL, &ev);
	return ev;
}

cl_event Image::queueRead(void* destination, cl_event wait_event) {
	cl_event ev;
	clEnqueueReadImage(_env.txQueue(), _block, CL_FALSE, _origin, _endpoint, 0, 0, destination, 1, &wait_event, &ev);
	return ev;
}

cl_event Image::queueWrite(void* source, cl_event wait_event) {
	cl_event ev;
	clEnqueueWriteImage(_env.txQueue(), _block, CL_FALSE, _origin, _endpoint, 0, 0, source, 1, &wait_event, &ev);
	return ev;
}

Image const& Image::operator=(Image const& other) throw(string) {

	_env = other._env;
	_m_type = other._m_type;
	_channels = other._channels;
	_pix_fmt = other._pix_fmt;
	_dimensions = other._dimensions;
	_format = other._format;
	_desc = other._desc;
	
	_origin[0] = _origin[1] = _origin[2] = 0;
	_endpoint[0] = _dimensions.width;
	_endpoint[1] = _dimensions.height;
	_endpoint[2] = 1;

	_size = other._size;
	_data = other._data;

	_block = other._block;
	clRetainMemObject(_block);

	return *this;

}
