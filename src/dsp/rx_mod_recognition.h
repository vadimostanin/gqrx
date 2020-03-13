/* -*- c++ -*- */
/*
 * Gqrx SDR: Software defined radio receiver powered by GNU Radio and Qt
 *           http://gqrx.dk/
 *
 * Copyright 2011-2020 Alexandru Csete OZ9AEC.
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
#ifndef RX_MOD_RECOGNITION_H
#define RX_MOD_RECOGNITION_H

#include <gnuradio/sync_block.h>
#include <gnuradio/fft/fft.h>
#include <gnuradio/filter/firdes.h>       /* contains enum win_type */
#include <gnuradio/gr_complex.h>
#include <boost/thread/mutex.hpp>
#include <boost/circular_buffer.hpp>
#include <chrono>


#define MAX_FFT_SIZE 1048576

class rx_mod_recogn;

typedef boost::shared_ptr<rx_mod_recogn> rx_mod_recogn_sptr;


/*! \brief Return a shared_ptr to a new instance of rx_fft_c.
 *  \param fftsize The FFT size
 *  \param winttype The window type (see gnuradio/filter/firdes.h)
 *
 * This is effectively the public constructor. To avoid accidental use
 * of raw pointers, the rx_fft_c constructor is private.
 * make_rx_fft_c is the public interface for creating new instances.
 */
rx_mod_recogn_sptr make_rx_mod_recogn(unsigned int fftsize=4096, double quad_rate=0, int wintype=gr::filter::firdes::WIN_HAMMING);


/*! \brief Block for computing complex FFT.
 *  \ingroup DSP
 *
 * This block is used to compute the FFT of the received spectrum.
 *
 * The samples are collected in a cicular buffer with size FFT_SIZE.
 * When the GUI asks for a new set of FFT data via get_fft_data() an FFT
 * will be performed on the data stored in the circular buffer - assuming
 * of course that the buffer contains at least fftsize samples.
 *
 * \note Uses code from qtgui_sink_c
 */
class rx_mod_recogn : public gr::sync_block
{
    friend rx_mod_recogn_sptr make_rx_mod_recogn(unsigned int fftsize, double quad_rate, int wintype);

protected:
    rx_mod_recogn(unsigned int fftsize=4096, double quad_rate=0, int wintype=gr::filter::firdes::WIN_HAMMING);

public:
    ~rx_mod_recogn();

    int work(int noutput_items,
             gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);

    void get_fft_data(std::complex<float>* fftPoints, unsigned int &fftSize);

    void set_window_type(int wintype);
    int  get_window_type() const;

    void set_fft_size(unsigned int fftsize);
    void set_quad_rate(double quad_rate);
    unsigned int get_fft_size() const;

private:
    unsigned int d_fftsize;   /*! Current FFT size. */
    double       d_quadrate;
    int          d_wintype;   /*! Current window type. */

    boost::mutex d_mutex;  /*! Used to lock FFT output buffer. */

    gr::fft::fft_complex    *d_fft;    /*! FFT object. */
    std::vector<float>  d_window; /*! FFT window taps. */

    boost::circular_buffer<gr_complex> d_cbuf; /*! buffer to accumulate samples. */
    std::chrono::time_point<std::chrono::steady_clock> d_lasttime;

    void do_fft(unsigned int size);
    void set_params();

};

#endif /* RX_MOD_RECOGNITION_H */
