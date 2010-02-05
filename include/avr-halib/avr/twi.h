template <class TWIRegmap> class TWIMaster
{
	union Slave_Adress{
		uint8_t raw_data;
		struct{
			bool read:1;
			uint8_t adress:7;
		};
	};
protected:
	typedef class TWIRegmap::Controller_Configuration Controller_Configuration;
public:
	typedef TWIRegmap Regmap;
	
	void init(uint8_t br)
	{
		UseRegmap(rm, TWIRegmap);
		rm.twen=true;
		SyncRegmap(rm);
		rm.twbr=br;
		rm.twcr=0x00;
		rm.twint=false;
		rm.twea=true;
		rm.twsta=false;
		rm.twsto=false;
		rm.twwc=false;
		rm.twen=true;
		rm.twie=false;
		
		rm.twsr=0x00;
		rm.twps=rm.ps1;
		SyncRegmap(rm);
	}
	/// There are two posibilitis of writing i2c adresses 8 bit baseadress or 7 bit adress u have to use the 7 bit adress here the shifting is done internal (rightshift ur 8 bit baseadress once)
	int start(uint8_t adress,bool read)
	{
		UseRegmap(rm, TWIRegmap);
		rm.twen=true;
		rm.twsta=true;
		rm.twint=true;
		SyncRegmap(rm);
		do{SyncRegmap(rm);}while(!rm.twint);
		switch(rm.tws)
		{
			case Regmap::st_start:
			case Regmap::repeat_start:
				break ;
			default:return false;
		}
		Slave_Adress ad;
		ad.adress = adress;
		ad.read = read;
		rm.twdr = ad.raw_data;
// 		rm.twdr = adress << 1 | read?TWIRegmap::read:TWIRegmap::write;
		rm.twsta=false;
		rm.twint=true;
		SyncRegmap(rm);
		do{SyncRegmap(rm);}while(!rm.twint);
		switch(rm.tws)
		{
			case Regmap::sla_write_ack:
			case Regmap::sla_read_ack:
				return true;
				break ;
			default:return false;
		}
		
	}
	
	void stop()
	{
		UseRegmap(rm, TWIRegmap);
		rm.twint=true;
		rm.twen=true;
		rm.twsto=true;
		SyncRegmap(rm);
	}
	 
	bool write(uint8_t byte)
	{	
		UseRegmap(rm, TWIRegmap);
		rm.twdr = byte;
		rm.twen = true;
		rm.twint = true;
		SyncRegmap(rm);
		do{SyncRegmap(rm);}while(!rm.twint);
		switch(rm.tws)
		{
			case Regmap::m_data_tx_ack:
			case Regmap::sl_data_tx_ack:
				return true;
				break;
			default:return false;
		}
	}
	uint8_t st()
	{
		UseRegmap(rm, TWIRegmap);
		SyncRegmap(rm);
		return rm.tws;
	}

	
	uint8_t read(bool last_byte)
	{
		UseRegmap(rm, TWIRegmap);
		rm.twea = !last_byte;
		rm.twint = true;
		SyncRegmap(rm);
		do{SyncRegmap(rm);}while(!rm.twint);
		return rm.twdr;
	}
};
