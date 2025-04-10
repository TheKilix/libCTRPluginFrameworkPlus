#ifndef CURLINC_TYPECHECK_GCC_H
#define CURLINC_TYPECHECK_GCC_H
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2019, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

/* wraps curl_easy_setopt() with typechecking */

/* To add a new kind of warning, add an
 *   if(curlcheck_sometype_option(_curl_opt))
 *     if(!curlcheck_sometype(value))
 *       _curl_easy_setopt_err_sometype();
 * block and define curlcheck_sometype_option, curlcheck_sometype and
 * _curl_easy_setopt_err_sometype below
 *
 * NOTE: We use two nested 'if' statements here instead of the && operator, in
 *       order to work around gcc bug #32061.  It affects only gcc 4.3.x/4.4.x
 *       when compiling with -Wlogical-op.
 *
 * To add an option that uses the same type as an existing option, you'll just
 * need to extend the appropriate _curl_*_option macro
 */


#endif /* CURLINC_TYPECHECK_GCC_H */
