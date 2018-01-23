#include "stdio.h"
#include "Drv_w25qxx.h"

typedef struct
{
	uint8_t Manufacturer;            /* Manufacturer */		// MF7-MF0(BYTE2)
	uint8_t Memory;               	 /* Density Code */		// ID15-ID8(BYTE3)
	uint8_t Capacity;                /* Family Code  */		// ID7-ID0(BYTE4)
	uint8_t rev;
}jedec_id_t;

/* Private define ------------------------------------------------------------*/
//#define W25QXX_DEBUG
#ifdef W25QXX_DEBUG
#define w25qxx_debug(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#else
#define w25qxx_debug(fmt, ...)
#endif

// ������ ID
#define JEDEC_MANUFACTURER_ST       0x20
#define JEDEC_MANUFACTURER_MACRONIX 0xC2
#define JEDEC_MANUFACTURER_WINBOND  0xEF	// �鿴�����ֲ�.

/* JEDEC Device ID: Memory type and Capacity */
#define JEDEC_W25Q16_BV_CL_CV   (0x4015) /* W25Q16BV W25Q16CL W25Q16CV  */
#define JEDEC_W25Q16_DW         (0x6015) /* W25Q16DW  */
#define JEDEC_W25Q32_BV         (0x4016) /* W25Q32BV */
#define JEDEC_W25Q32_DW         (0x6016) /* W25Q32DW */
#define JEDEC_W25Q64_BV_CV      (0x4017) /* W25Q64BV W25Q64CV */
#define JEDEC_W25Q64_DW         (0x4017) /* W25Q64DW */
#define JEDEC_W25Q128_BV        (0x4018) /* W25Q128BV */

#define JEDEC_WRITE_ENABLE           0x06
#define JEDEC_WRITE_DISABLE          0x04
#define JEDEC_READ_STATUS            0x05
#define JEDEC_WRITE_STATUS           0x01
#define JEDEC_READ_DATA              0x03
#define JEDEC_FAST_READ              0x0b
#define JEDEC_DEVICE_ID              0x9F
#define JEDEC_PAGE_WRITE             0x02
#define JEDEC_SECTOR_ERASE           0x20

#define JEDEC_STATUS_BUSY            0x01
#define JEDEC_STATUS_WRITEPROTECT    0x02
#define JEDEC_STATUS_BP0             0x04
#define JEDEC_STATUS_BP1             0x08
#define JEDEC_STATUS_BP2             0x10
#define JEDEC_STATUS_TP              0x20
#define JEDEC_STATUS_SEC             0x40
#define JEDEC_STATUS_SRP0            0x80

#define DUMMY_BYTE     0xFF			 // ���ֽ�.

// PA4: SPI1_NSS
#define W25QXX_CS_LOW()      GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET)
#define W25QXX_CS_HIGH()     GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET)

static flash_info_t flash_info;
static uint8_t Flash_ReadID ( jedec_id_t *id );
static uint8_t Flash_SendByte ( uint8_t byte );
static void Flash_WaitForEnd (void);


//---------------------------- function ------------------------------//
void Flash_Init(void)
{
	jedec_id_t flash_id;
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

	// GPIO & SPI1 rcc open
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// PA5\PA6\PA7 -- SCK\MISO\MOSI
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	//SPI GPIO Configuration
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;		// ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// �������
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;		// ����
    GPIO_Init(GPIOA, &GPIO_InitStructure );

	//flash SPI CS
    GPIO_InitStructure.GPIO_Pin             = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode            = GPIO_Mode_OUT ;   // ���
    GPIO_InitStructure.GPIO_OType           = GPIO_OType_PP;	// ����
    GPIO_InitStructure.GPIO_PuPd            = GPIO_PuPd_UP;		// ����
    GPIO_InitStructure.GPIO_Speed           = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure );
	
	//SPI configuration
    SPI_I2S_DeInit(SPI1);
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;	// ����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;					// ����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;					// ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;					// ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;					// ���ݲ����ڵڶ���ʱ����
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;						// NSS�ź���Ӳ��(NSS�ܽ�)�������(ʹ��SSIλ)����:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;			// ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ 16
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;					// ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial     = 7;								// CRCֵ����Ķ���ʽ
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
    W25QXX_CS_HIGH();		// CS_HIGH
	
	/* Select the FLASH: Chip Select low */    
	W25QXX_CS_LOW();		// CS_LOW
	/* Send "0xff " instruction */
    Flash_SendByte(DUMMY_BYTE);		// ����һ�����ֽ�.
	
	W25QXX_CS_HIGH();		

	/* Read Flash Id */
    Flash_ReadID(&flash_id);		// Manufacturer id

	if(flash_id.Manufacturer == JEDEC_MANUFACTURER_WINBOND)		// �ж� id
	{
		flash_info.sector_size = 4096;                         /* Page Erase (4096 Bytes) */
		if(flash_id.Capacity == (JEDEC_W25Q128_BV & 0xff))
		{
			w25qxx_debug ( "W25Q128_BV detection\r\n" );
            flash_info.sector_count = 4096;                    /* 128Mbit / 8 / 4096 = 4096 */
		}
		else if(flash_id.Capacity == (JEDEC_W25Q64_DW & 0xff))
		{
            w25qxx_debug ( "W25Q64_DW or W25Q64_BV or W25Q64_CV detection\r\n" );
			flash_info.sector_count = 2048;					   // 64Mbit / 8 / 4096 = 2048
		}
		else if(flash_id.Capacity == (JEDEC_W25Q32_DW & 0xff))
		{
			w25qxx_debug ( "W25Q32_DW or W25Q32_BV detection\r\n" );
			flash_info.sector_count = 1024;					   // 32Mbit / 8 / 4096 = 1024
		}
		else if(flash_id.Capacity == (JEDEC_W25Q16_DW & 0xff))
		{
			w25qxx_debug ( "W25Q16_DW or W25Q16_BV detection\r\n" );
			flash_info.sector_count = 512;					   // 16Mbit / 8 / 4096 = 512
		}
		else
		{
			w25qxx_debug ( "error flash capacity\r\n" );
            flash_info.sector_count = 0;
		}
		
		// flash ����   capacity = sector_size * sector_count
		flash_info.capacity = flash_info.sector_size * flash_info.sector_count;
	}
	else 
	{
		w25qxx_debug ( "Unknow Manufacturer ID!%02X\r\n", flash_id.Manufacturer );
        flash_info.initialized = 0;
        return;
	}
	   
	flash_info.initialized = 1;
}

/***************************************************************************
* ԭ�� Prototype: static uint8_t Flash_SendByte (uint8_t byte)
* ���� Function : Flash_SendByte
* ���� Parameter: uint8_t byte
* ����ֵ Returned value: uint8_t ���ش� SPI �����϶�ȡ���ֽ�
****************************************************************************/
static uint8_t Flash_SendByte (uint8_t byte)
{
    /* Loop while DR register in not emplty */
    while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_TXE ) == RESET );

    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData ( SPI1, byte );

    /* Wait to receive a byte */
    while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_RXNE ) == RESET );

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData (SPI1);
}

/***************************************************************************
* ԭ�� Prototype: uint8_t Flash_ReadID ( jedec_id_t *id )
* ���� Function : Reads FLASH identification.
* ���� Parameter: jedec_id_t *id
* ����ֵ Returned value: Manufacturer id
****************************************************************************/
uint8_t Flash_ReadID(jedec_id_t *id)
{
	uint8_t *recv_buffer = ( uint8_t* ) id;

    /* Select the FLASH: Chip Select low */
    W25QXX_CS_LOW();		// ��ʼͨѶ: CS �͵�ƽ

    /* Send "RDID " instruction */
    Flash_SendByte (JEDEC_DEVICE_ID);		//  ���� JEDEC_DEVICE_ID ָ��, ��ȡ ID --- 0x9F

    /* Read a byte from the FLASH */
    *recv_buffer++ = Flash_SendByte ( DUMMY_BYTE );	// ��ȡһ���ֽ�

    /* Read a byte from the FLASH */
    *recv_buffer++ = Flash_SendByte ( DUMMY_BYTE );	// ��ȡһ���ֽ�

    /* Read a byte from the FLASH */
    *recv_buffer++ = Flash_SendByte ( DUMMY_BYTE ); // ��ȡһ���ֽ�

    /* Deselect the FLASH: Chip Select high */
    W25QXX_CS_HIGH();		// ֹͣͨѶ, CS �ߵ�ƽ
	
	return id->Manufacturer;	// �� id �����ڽṹ�� ID -> Manufacturer 
}

/***************************************************************************
* ԭ�� Prototype: static void Flash_WriteEnable (void)
* ���� Function : Flash_WriteEnable
* ���� Parameter: none
* ����ֵ Returned value: none
****************************************************************************/
static void Flash_WriteEnable (void)
{
    /* Select the FLASH: Chip Select low */
    W25QXX_CS_LOW();
    /* Send Write Enable instruction */
    Flash_SendByte ( JEDEC_WRITE_ENABLE );
    /* Deselect the FLASH: Chip Select high */
    W25QXX_CS_HIGH();
}

/**
  * @brief �ȴ� WIP(BUSY) ��־λ���� 0, ���ȵ� FLASH �ڲ�����д�����
  * @param none
  * @retval none
  */
static void Flash_WaitForEnd (void)
{
    u8 FLASH_Status = 0;	

    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        /* Select the FLASH: Chip Select low */
        W25QXX_CS_LOW();
        /* Send "Read Status Register" instruction */
        Flash_SendByte ( JEDEC_READ_STATUS );			// ���Ͷ�ȡ ״̬�Ĵ��� ָ��
        /* Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        FLASH_Status = Flash_SendByte ( DUMMY_BYTE );	// ��ȡ FLASH оƬ��״̬�Ĵ���
        /* Deselect the FLASH: Chip Select high */
        W25QXX_CS_HIGH();
    }
    while ( FLASH_Status & JEDEC_STATUS_BUSY );			// �ȴ� FLASH  д�����
}

// Flash Sectors Read
void Flash_SectorsRead (uint32_t address, uint8_t *buffer, uint16_t count)
{
	uint16_t i = 0;
	
	for(i = 0; i < count; i++)
	{
		Flash_PageRead(address,buffer,flash_info.sector_size);
		buffer += flash_info.sector_size;
		address += flash_info.sector_size;
	}
}

/***************************************************************************
* ԭ�� Prototype: void Flash_SectorErase (uint32_t address, uint8_t state)
* ���� Function : Erases the specified FLASH sector. ����ָ��������.
* ���� Parameter: uint32_t address, uint8_t state
* ����ֵ Returned value: none
****************************************************************************/
void Flash_SectorErase (uint32_t address, uint8_t state)
{
    Flash_WriteEnable();		// дʹ��, �ڲ���֮ǰ
    /* Select the FLASH: Chip Select low */
    W25QXX_CS_LOW();			// CS_LOW
    /* Send Sector Erase instruction */
    Flash_SendByte (JEDEC_SECTOR_ERASE);		// ������������ָ�� 0x20
    /* Send SectorAddr high nibble address byte */
    Flash_SendByte ((address & 0xFF0000) >> 16);		// ���Ͳ���������ַ�ĸ�λ
    /* Send SectorAddr medium nibble address byte */
    Flash_SendByte ((address & 0xFF00) >> 8);			// ���Ͳ���������ַ����λ
    /* Send SectorAddr low nibble address byte */
    Flash_SendByte (address & 0xFF);					// ���Ͳ���������ַ�ĵ�λ
    /* Deselect the FLASH: Chip Select high */
    W25QXX_CS_HIGH();			// CS_HIGH

    /* Wait the end of Flash writing */
    if (state)				// �ȴ��������
    {
        Flash_WaitForEnd();
    }
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE
  *         cycle(Page WRITE sequence). The number of byte can't exceed
  *         the FLASH page size.
  * @param pBuffer : pointer to the buffer  containing the data to be
  *                  written to the FLASH.
  * @param WriteAddr : FLASH's internal address to write to.
  * @param NumByteToWrite : number of bytes to write to the FLASH,
  *                       must be equal or less than "SPI_FLASH_PageSize" value.
  * @retval : None
  */
void Flash_PageWrite (uint32_t address, uint8_t* buffer,  uint32_t lenght)
{
    Flash_WriteEnable();			// дʹ��  
    /* Select the FLASH: Chip Select low */
    W25QXX_CS_LOW();				// CS_LOW
    /* Send "Write to Memory " instruction */
    Flash_SendByte (JEDEC_PAGE_WRITE);				// ���Ͳ���ָ�� 0x02
    /* Send WriteAddr high nibble address byte to write to */
    Flash_SendByte ((address & 0xFF0000) >> 16);	// ����д��ַ��λ
    /* Send WriteAddr medium nibble address byte to write to */
    Flash_SendByte ((address & 0xFF00) >> 8);		// ����д��ַ��λ
    /* Send WriteAddr low nibble address byte to write to */
    Flash_SendByte (address & 0xFF);				// ����д��ַ��λ

    /* while there is data to be written on the FLASH */
    while (lenght--)				// д������
    {
        /* Send the current byte */
        Flash_SendByte (*buffer);	// ����ǰҪд�������
        /* Point on the next byte to be written */
        buffer++;					// ָ����һ������
    }

    /* Deselect the FLASH: Chip Select high */
    W25QXX_CS_HIGH();				// CS_HIGH

    /* Wait the end of Flash writing */
    Flash_WaitForEnd();				// ��ѯ FLASH ״̬, �ȴ� FLASH д���.
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param buffer : pointer to the buffer that receives the data read
  *                  from the FLASH.
  * @param address : FLASH's internal address to read from.
  * @param lenght : number of bytes to read from the FLASH.
  * @retval : None
  */
void Flash_PageRead (uint32_t address, uint8_t* buffer,  uint32_t lenght)
{
    /* Select the FLASH: Chip Select low */
    W25QXX_CS_LOW();
	
	/* Send "Read from Memory " instruction */
	Flash_SendByte ( JEDEC_READ_DATA );		// ���Ͷ�����ָ��  0x03
	
	/* Send ReadAddr high nibble address byte to read from */
    Flash_SendByte ( ( address & 0xFF0000 ) >> 16 );
    /* Send ReadAddr medium nibble address byte to read from */
    Flash_SendByte ( ( address & 0xFF00 ) >> 8 );
    /* Send ReadAddr low nibble address byte to read from */
    Flash_SendByte ( address & 0xFF );
	
	while ( lenght-- ) /* while there is data to be read */
    {
        /* Read a byte from the FLASH */
        *buffer = Flash_SendByte ( DUMMY_BYTE );		// ����� buffer ��ȡ������ֵ���浽 �ṹ����
        /* Point to the next location where the byte read will be saved */
        buffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    W25QXX_CS_HIGH();
}
