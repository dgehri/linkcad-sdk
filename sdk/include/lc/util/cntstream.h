//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once

#include <streambuf>

namespace lc::util {

//------------------------------------------------------------------------------
// std::istream derived class with line and byte counting
//------------------------------------------------------------------------------
// Usage
//
//    std::ifstream    file;
//    cntstreambuf     cntstreambuf(file.rdbuf());
//    std::istream     is(&cntstreambuf);
//
// then use "is" instead of "file".
//------------------------------------------------------------------------------
class cntstreambuf final : public std::streambuf
{
public:
    // constructor
    explicit cntstreambuf(std::streambuf* sbuf)
        : streamBuf_(sbuf)
        , lineNumber_(1)
        , lastLineNumber_(1)
        , column_(0)
        , prevColumn_(static_cast<unsigned int>(-1))
        , filePos_(0)
    {}

    // Get current line number
    [[nodiscard]] unsigned int lineno() const { return lineNumber_; }

    // Get line number of last read character
    [[nodiscard]] unsigned int lastLineNumber() const { return lastLineNumber_; }

    // Get current column
    [[nodiscard]] unsigned int colno() const { return column_; }

    // Get file position
    [[nodiscard]] std::streamsize filepos() const { return filePos_; }

    cntstreambuf(const cntstreambuf&) = delete;
    cntstreambuf& operator=(const cntstreambuf&) = delete;

protected:
    // extract next character from stream w/o advancing read pos
    std::streambuf::int_type underflow() override { return streamBuf_->sgetc(); }

    // extract next character from stream
    std::streambuf::int_type uflow() override
    {
        const auto rc = streamBuf_->sbumpc();

        lastLineNumber_ = lineNumber_;
        if (traits_type::eq_int_type(rc, traits_type::to_int_type('\n')))
        {
            ++lineNumber_;
            prevColumn_ = column_ + 1;
            column_ = static_cast<unsigned int>(-1);
        }

        ++column_;
        ++filePos_;
        return rc;
    }

    // put back last character
    std::streambuf::int_type pbackfail(std::streambuf::int_type c) override
    {
        if (traits_type::eq_int_type(c, traits_type::to_int_type('\n')))
        {
            --lineNumber_;
            lastLineNumber_ = lineNumber_;
            column_ = prevColumn_;
            prevColumn_ = 0;
        }

        --column_;
        --filePos_;

        if (c != traits_type::eof())
            return streamBuf_->sputbackc(traits_type::to_char_type(c));

        return streamBuf_->sungetc();
    }

    // change position by offset, according to way and mode
    std::ios::pos_type seekoff(std::ios::off_type pos,
                               std::ios_base::seekdir dir,
                               std::ios_base::openmode mode) override
    {
        if (dir == std::ios_base::beg && pos == static_cast<std::ios::off_type>(0))
        {
            lastLineNumber_ = 1;
            lineNumber_ = 1;
            column_ = 0;
            prevColumn_ = static_cast<unsigned int>(-1);
            filePos_ = 0;

            return streamBuf_->pubseekoff(pos, dir, mode);
        }

        return std::streambuf::seekoff(pos, dir, mode);
    }

    // change to specified position, according to mode
    std::ios::pos_type seekpos(std::ios::pos_type pos, std::ios_base::openmode mode) override
    {
        if (pos == static_cast<std::ios::pos_type>(0))
        {
            lastLineNumber_ = 1;
            lineNumber_ = 1;
            column_ = 0;
            prevColumn_ = static_cast<unsigned int>(-1);
            filePos_ = 0;

            return streamBuf_->pubseekpos(pos, mode);
        }

        return std::streambuf::seekpos(pos, mode);
    }


private:
    std::streambuf* streamBuf_;  // hosted streambuffer
    unsigned int lineNumber_;  // current line number
    unsigned int lastLineNumber_;  // line number of last read character
    unsigned int column_;  // current column
    unsigned int prevColumn_;  // previous column
    std::streamsize filePos_;  // file position
};

}  // namespace lc::util
