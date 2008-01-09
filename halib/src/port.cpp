//volatile port_t & port;
//volatile port_t & direction;

SimplePort::SimplePort(volatile port_t & port, volatile port_t & pin, volatile port_t & ddr):
	this.port(port),this.pin(pin) ,this.ddr(ddr)
{}


void SimplePort::set(const port_t pattern)
{
	port=pattern
}

port_t SimplePort::get()
{
	return port;
}

port_t getPin()
{}

void SimplePort::setDirection(const port_t pattern);
{
	ddr=pattern;
}
port_t SimplePort::getDirection(const port_t pattern);
{
	return ddr;
}



public:
ShiftedPort::ShiftedPort(volatile port_t & port, volatile port_t & ddr, const port_t mask, const uint8_t shift):
	this.port(port), this.ddr(ddr), this.mask(mask),this.shift(shift)
{}
void ShiftedPort::set(const port_t pattern)
{
	port_t spattern= (pattern) << shift;
	port = (spattern & mask) | (port & ~mask); 
}
port_t ShiftedPort::get()
{
	return (port & mask) >> shift;
}
void ShiftedPort::setDirection(const port_t pattern);
port_t ShiftedPort::getDirection(const port_t pattern);
