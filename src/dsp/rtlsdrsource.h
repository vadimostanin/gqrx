#ifndef RTLSDRSOURCE_H
#define RTLSDRSOURCE_H

/* -*- c++ -*- */
/*
 * Gqrx SDR: Software defined radio receiver powered by GNU Radio and Qt
 *           http://gqrx.dk/
 *
 * Copyright 2020 Vadym Ostanin.
 *
 * Gqrx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gqrx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gqrx; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#include <gnuradio/sync_block.h>
#include <gnuradio/fft/fft.h>
#include <gnuradio/filter/firdes.h>       /* contains enum win_type */
#include <gnuradio/gr_complex.h>
#include <boost/thread/mutex.hpp>
#include <boost/circular_buffer.hpp>
#include <chrono>


#define MAX_FFT_SIZE 1048576

class rtlsdrsource;

typedef boost::shared_ptr<rtlsdrsource> rtlsdrsource_sptr;


/*! \brief Return a shared_ptr to a new instance of rtlsdrsource.
 *  \param devindex rtlsdr device index
 *
 */
rtlsdrsource_sptr make_rtlsdrsource(int32_t devindex);


/*! \brief Block for accessing RTLSDR.
 *  \ingroup DSP
 *
 */
class rtlsdrsource : public gr::sync_block
{
    friend rtlsdrsource_sptr make_rtlsdrsource(int32_t devindex);

protected:
    rtlsdrsource(int32_t devindex);

public:
    ~rtlsdrsource();

    int work(int noutput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);

private:

    int32_t m_devIndex;
    boost::mutex d_mutex;  /*! Used to lock FFT output buffer. */

    boost::circular_buffer<gr_complex> d_cbuf; /*! buffer to accumulate samples. */
    std::chrono::time_point<std::chrono::steady_clock> d_lasttime;
};

#endif // RTLSDRSOURCE_H
