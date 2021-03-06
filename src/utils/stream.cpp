#include <utils/stream.hpp>

ByteOStream::ByteOStream()
{
}

ByteOStream::ByteOStream(std::string filename)
{
    open(filename);
}

ByteOStream::ByteOStream(std::ostream& os)
{
    m_stream_ptr.reset(&os);
}

void ByteOStream::open(std::string filename)
{
    m_stream_ptr.reset(new std::ofstream(filename, std::ios::binary | std::ios::out));
}

bool ByteOStream::good()
{
    return m_stream_ptr->good();
}

void ByteOStream::Write(md::float3 pos, md::float3 vel, md::float3 accel)
{
    if (!(m_ignore_flags & StreamIgnore::IGNORE_POS)) {
        *m_stream_ptr << pos;
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_VEL)) {
        *m_stream_ptr << vel;
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_ACCEL)) {
        *m_stream_ptr << accel;
    }
}

void ByteOStream::Write(cl_float3 pos, cl_float3 vel, cl_float3 accel)
{
    if (!(m_ignore_flags & StreamIgnore::IGNORE_POS)) {
        *m_stream_ptr << pos.s[0] << pos.s[1] << pos.s[2];
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_VEL)) {
        *m_stream_ptr << vel.s[0] << vel.s[1] << vel.s[2];
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_ACCEL)) {
        *m_stream_ptr << accel.s[0] << accel.s[1] << accel.s[2];
    }
}

ByteIStream::ByteIStream()
{
}

ByteIStream::ByteIStream(std::string filename)
{
    open(filename);
}

ByteIStream::ByteIStream(std::istream& os)
{
    m_stream_ptr.reset(&os);
}

void ByteIStream::open(std::string filename)
{
    m_stream_ptr.reset(new std::ifstream(filename, std::ios::binary | std::ios::in));
}

bool ByteIStream::good()
{
    return m_stream_ptr->good();
}

void ByteIStream::Read(md::float3& pos, md::float3& vel, md::float3& accel)
{
    if (!(m_ignore_flags & StreamIgnore::IGNORE_POS)) {
        *m_stream_ptr >> pos;
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_VEL)) {
        *m_stream_ptr >> vel;
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_ACCEL)) {
        *m_stream_ptr >> accel;
    }
}

void ByteIStream::Read(cl_float3& pos, cl_float3& vel, cl_float3& accel)
{
    if (!(m_ignore_flags & StreamIgnore::IGNORE_POS)) {
        *m_stream_ptr >> pos.s[0] >> pos.s[1] >> pos.s[2];
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_VEL)) {
        *m_stream_ptr >> vel.s[0] >> vel.s[1] >> vel.s[2];
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_ACCEL)) {
        *m_stream_ptr >> accel.s[0] >> accel.s[1] >> accel.s[2];
    }
}

TextOStream::TextOStream()
{
}

TextOStream::TextOStream(std::string filename)
{
    open(filename);
}

TextOStream::TextOStream(std::ostream& os)
{
    m_stream_ptr.reset(&os);
}

void TextOStream::open(std::string filename)
{
    m_stream_ptr.reset(new std::ofstream(filename, std::ios::out));
}

bool TextOStream::good()
{
    return m_stream_ptr->good();
}

void TextOStream::Write(md::float3 pos, md::float3 vel, md::float3 accel)
{
    if (!(m_ignore_flags & StreamIgnore::IGNORE_POS)) {
        *m_stream_ptr << pos << " ";
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_VEL)) {
        *m_stream_ptr << vel << " ";
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_ACCEL)) {
        *m_stream_ptr << accel << " ";
    }

    *m_stream_ptr << "\n";
}

void TextOStream::Write(cl_float3 pos, cl_float3 vel, cl_float3 accel)
{
    if (!(m_ignore_flags & StreamIgnore::IGNORE_POS)) {
        *m_stream_ptr << pos.s[0] << " " << pos.s[1] << " " << pos.s[2] << " ";
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_VEL)) {
        *m_stream_ptr << vel.s[0] << " " << vel.s[1] << " " << vel.s[2] << " ";
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_ACCEL)) {
        *m_stream_ptr << accel.s[0] << " " << accel.s[1] << " " << accel.s[2] << " ";
    }

    *m_stream_ptr << "\n";
}

TextIStream::TextIStream()
{
}

TextIStream::TextIStream(std::string filename)
{
    open(filename);
}

TextIStream::TextIStream(std::istream& os)
{
    m_stream_ptr.reset(&os);
}

void TextIStream::open(std::string filename)
{
    m_stream_ptr.reset(new std::ifstream(filename, std::ios::in));
}

bool TextIStream::good()
{
    return m_stream_ptr->good();
}

void TextIStream::Read(md::float3& pos, md::float3& vel, md::float3& accel)
{
    if (!(m_ignore_flags & StreamIgnore::IGNORE_POS)) {
        *m_stream_ptr >> pos;
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_VEL)) {
        *m_stream_ptr >> vel;
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_ACCEL)) {
        *m_stream_ptr >> accel;
    }
}

void TextIStream::Read(cl_float3& pos, cl_float3& vel, cl_float3& accel)
{
    if (!(m_ignore_flags & StreamIgnore::IGNORE_POS)) {
        *m_stream_ptr >> pos.s[0] >> pos.s[1] >> pos.s[2];
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_VEL)) {
        *m_stream_ptr >> vel.s[0] >> vel.s[1] >> vel.s[2];
    }

    if (!(m_ignore_flags & StreamIgnore::IGNORE_ACCEL)) {
        *m_stream_ptr >> accel.s[0] >> accel.s[1] >> accel.s[2];
    }
}

StringStream::StringStream()
{
    m_stream_ptr.reset(&m_ss, [](decltype(m_ss)* ptr){});
}


ParticleIStreamPtr StreamFactory::MakeTraceIStream(TraceConfig conf)
{
    if (conf.binary_file) {
        return std::make_shared<ByteIStream>(conf.filename);
    } else {
        return std::make_shared<TextIStream>(conf.filename);
    }
}
ParticleOStreamPtr StreamFactory::MakeTraceOStream(TraceConfig conf)
{
    if (conf.binary_file) {
        return std::make_shared<ByteOStream>(conf.filename);
    } else {
        return std::make_shared<TextOStream>(conf.filename);
    }
}

ParticleIStreamPtr StreamFactory::MakeInitIStream(ParticleSystemConfig conf)
{
    if (conf.init_file_binary) {
        return std::make_shared<ByteIStream>(conf.init_file);
    } else {
        return std::make_shared<TextIStream>(conf.init_file);
    }
}

ParticleOStreamPtr StreamFactory::MakeResultOStream(ParticleSystemConfig conf)
{
    if (conf.result_file_binary) {
        return std::make_shared<ByteOStream>(conf.result_file);
    } else {
        return std::make_shared<TextOStream>(conf.result_file);
    }
}
