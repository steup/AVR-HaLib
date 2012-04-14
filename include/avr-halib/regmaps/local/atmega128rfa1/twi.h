#pragma once

#include <avr-halib/regmaps/base/localRegMap.h>

namespace avr_halib {
namespace regmaps {
namespace local {
namespace atmega128rfa1 {

    struct TWI : public base::LocalRegMap
    {
        public:
		union{
			struct{
				uint8_t __base [0xb8];
			
				uint8_t twbr;
				union{
					uint8_t twsr;
					struct
					{
						uint8_t twps :2;
						uint8_t :1;
						uint8_t tws :5;
					};
				};
				union{
					uint8_t twar;
					struct{
					bool twgce :1;
					uint8_t twa :7;
					};
				};
				uint8_t twdr;
				union{
					uint8_t twcr;
					struct{
					bool twie :1;
					bool :1;
					bool twen :1;
					bool twwc :1;
					bool twsto :1;
					bool twsta :1;
					bool twea :1;
			 		bool twint :1;
					};
				};
			};
			
		};
	
		
		enum {ps1 = 0, ps4 = 1, ps16 = 2, ps64 = 3};
		enum {read = 1 , write = 0};
		enum status{
			st_start	= 1,
			repeat_start	= 2,
			sla_write_ack	= 3,
			sla_write_noack	= 4,
			m_data_tx_ack	= 5,
			m_data_tx_noack	= 6,
			sla_w_buslost	= 7,
			sla_read_ack	= 8,
			sla_read_noack	= 9,
			m_data_rx_ack	= 10,
			m_data_rx_noack	= 11,
			sla_w_recived	= 12,
			buslost_sla_w_recived	= 13,
			gc_w_recived	= 14,
			buslost_gc_w_recived	= 15,
			sl_data_rx_ack	= 16,
			sl_data_rx_noack	= 17,
			gc_data_rx_ack	= 18,
			gc_data_rx_noack	= 19,
			stop_recived	= 20,
			sla_r_recived	= 21,
			buslost_sla_r_recived	= 22,
			sl_data_tx_ack	= 23,
			sl_data_tx_noack	= 24,
			sl_data_tx_ack_last	= 25,
			no_status	= 31,
			bus_error	= 0
		};

    } __attribute__((packed));
}
}
}
}

