#ifndef _DYNAMIXEL_HEADER
#define _DYNAMIXEL_HEADER


#ifdef __cplusplus
extern "C" {
#endif

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46
#define MOVING_SPEED    32
#define DEFAULT_BAUDNUM    1



///////////// device control methods ////////////////////////
int dxl_initialize(int deviceIndex, int baudnum );
void dxl_terminate();


///////////// set/get packet methods //////////////////////////
#define MAXNUM_TXPARAM		(150)
#define MAXNUM_RXPARAM		(60)

void dxl_set_txpacket_id(int id);
#define BROADCAST_ID		(254)

void dxl_set_txpacket_instruction(int instruction);
#define INST_PING			(1)
#define INST_READ			(2)
#define INST_WRITE			3
#define INST_REG_WRITE		(4)
#define INST_ACTION			(5)
#define INST_RESET			(6)
#define INST_SYNC_WRITE		(131)

void dxl_set_txpacket_parameter(int index, int value);
void dxl_set_txpacket_length(int length);

int dxl_get_rxpacket_error(int errbit);
#define ERRBIT_VOLTAGE		(1)
#define ERRBIT_ANGLE		(2)
#define ERRBIT_OVERHEAT		(4)
#define ERRBIT_RANGE		(8)
#define ERRBIT_CHECKSUM		(16)
#define ERRBIT_OVERLOAD		(32)
#define ERRBIT_INSTRUCTION	(64)

int dxl_get_rxpacket_length(void);
int dxl_get_rxpacket_parameter(int index);


// utility for value
int dxl_makeword(int lowbyte, int highbyte);
int dxl_get_lowbyte(int word);
int dxl_get_highbyte(int word);


////////// packet communication methods ///////////////////////
void dxl_tx_packet(void);
void dxl_rx_packet(void);
void dxl_txrx_packet(void);

int dxl_get_result(void);
#define	COMM_TXSUCCESS		(0)
#define COMM_RXSUCCESS		(1)
#define COMM_TXFAIL		(2)
#define COMM_RXFAIL		(3)
#define COMM_TXERROR		(4)
#define COMM_RXWAITING		(5)
#define COMM_RXTIMEOUT		(6)
#define COMM_RXCORRUPT		(7)


//////////// high communication methods ///////////////////////
void dxl_ping(int id);
int dxl_read_byte(int id, int address);
void dxl_write_byte(int id, int address, int value);
int dxl_read_word(int id, int address);
void dxl_write_word(int id, int address, int value);

#define KEEP_WALKING *(mem+2)

#define LEG_RIGHT 0
#define LEG_LEFT 1

//////////// extra functions ///////////////////////
void levantar_fitPC();
void espera_mov();
void robo_ereto();
void robo_ereto_corrigido();
void andar(int passos);
void andar_rapido_ik();
void virar(int passos);
void chutepedireito_fitPC();
void chutepedireito();
void inclinar();
void levanta_abaixa_perna_dir();
void levanta_abaixa_perna_esq();
void levantar_de_barriga();
void levantar_de_costas_fitPC();
void levantar_de_costas();
int visao();
float distancia_bola();
float angle_bola();
void andar_lateral_direita(int passos);
void andar_lateral_esquerda(int passos);
void andar_curto(int passos);
void andar_rapido(int passos);
void espera_mov_ID(int id);

void inverse_kinematic_right_leg(float x, float y_e, int inclin, int VEL);
void inverse_kinematic_left_leg(float x, float y_e, int inclin, int VEL);
void espera_mov_ID(int id);
void espera_mov();
void inclina_plano(int select_leg, int angle, int vel, int offset_pe);


////global variables/////////////////////////
extern int inclina;
extern unsigned int StandupPos[22];
extern unsigned int StandupPos_corrigido[22];
extern unsigned int StandupPos_corrigido2[22];
extern int *mem;



#ifdef __cplusplus
}
#endif

#endif
