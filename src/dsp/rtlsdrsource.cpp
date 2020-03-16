#include "rtlsdrsource.h"

/* -*- c++ -*- */
/*
 * Gqrx SDR: Software defined radio receiver powered by GNU Radio and Qt
 *           http://gqrx.dk/
 *
 * Copyright 2011-2013 Alexandru Csete OZ9AEC.
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
#include <math.h>
#include <gnuradio/io_signature.h>
#include <gnuradio/filter/firdes.h>
#include <gnuradio/gr_complex.h>
#include <gnuradio/fft/fft.h>
#include "dsp/rx_fft.h"
#include <algorithm>


rtlsdrsource_sptr make_rtlsdrsource (int32_t devindex)
{
    return gnuradio::get_initial_sptr(new rtlsdrsource (devindex));
}

/*! \brief Create receiver FFT object.
 *  \param fftsize The FFT size.
 *  \param wintype The window type (see gr::filter::firdes::win_type).
 *
 */
rtlsdrsource::rtlsdrsource(int32_t devindex)
    : gr::sync_block ("rtlsdrsource",
          gr::io_signature::make(0, 0, 0),
          gr::io_signature::make(1, 1, sizeof(gr_complex))),
      m_devIndex(devindex)
{
    d_lasttime = std::chrono::steady_clock::now();
}

rtlsdrsource::~rtlsdrsource()
{
}

/*! \brief Receiver FFT work method.
 *  \param noutput_items
 *  \param input_items
 *  \param output_items
 *
 * This method does nothing except throwing the incoming samples into the
 * circular buffer.
 * FFT is only executed when the GUI asks for new FFT data via get_fft_data().
 */
int rtlsdrsource::work(int noutput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items)
{
    auto in = static_cast<const gr_complex*>(input_items[0]);
    (void) output_items;

    /* just throw new samples into the buffer */
    boost::mutex::scoped_lock lock(d_mutex);
    for (int i = 0; i < noutput_items; i++)
    {
        d_cbuf.push_back(in[i]);
    }

    return noutput_items;

}
