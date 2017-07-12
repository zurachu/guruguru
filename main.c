#include <piece.h>

#include "Lcd.h"
#include "PrecisionTimer.h"

static PrecisionTimer g_timer;

typedef enum tagGameMode
{
	GameMode_InitializeFailure,
	GameMode_Title,
	GameMode_Ready,
	GameMode_Main,
	GameMode_Result
} GameMode;
static GameMode g_mode = GameMode_InitializeFailure;

static int g_count = 0;

typedef enum tagDirection
{
	Direction_Up,
	Direction_Right,
	Direction_Down,
	Direction_Left,
	
	Direction_Num
} Direction;
static Direction g_dir = Direction_Up;

static unsigned long const s_pad[ Direction_Num ] = { TRG_UP, TRG_RI, TRG_DN, TRG_LF };
static char const* const s_disp[ Direction_Num ] = { "↑", "→", "↓", "←" };

static int g_score = 0;

void ChangeMode( GameMode mode )
{
	if( g_mode != mode )
	{
		g_mode = mode;
		g_count = 0;
	}
}

void Disp( void )
{
	pceFontSetType( 0 );
	pceFontSetPos( 20, 20 );
	pceFontPutStr( s_disp[ g_dir ] );
	pceFontSetPos( 64, 20 );
	pceFontPrintf( "%2d回転 %d/4", g_score / 4, g_score % 4 );
}

void Title( void );
void Ready( void );
void Main( void );
void Result( void );

/// 初期化.
void pceAppInit( void )
{
	pceAppSetProcPeriod( 33 );
	
	if( Lcd_Init() )
	{
		pceFontSetTxColor( 3 );
		pceFontSetBkColor( FC_SPRITE );
		PrecisionTimer_Construct( &g_timer );
		ChangeMode( GameMode_Title );
	}
}

/// メイン.
void pceAppProc( int cnt )
{
	PrecisionTimer timer;
	PrecisionTimer_Construct( &timer );
	pceLCDPaint( 0, 0, 0, DISP_X, DISP_Y );
	switch( g_mode )
	{
	case GameMode_InitializeFailure:
		pceAppReqExit( 0 );
		break;
	case GameMode_Title:
		Title();
		break;
	case GameMode_Ready:
		Ready();
		break;
	case GameMode_Main:
		Main();
		break;
	case GameMode_Result:
		Result();
		break;
	}
	
	pceFontSetType( 2 );
	pceFontSetPos( 0, 82 );
	pceFontPrintf( "%8lu/%8luus FREE:%8d", PrecisionTimer_Count( &timer ),
		PrecisionTimer_Count( &g_timer ), pceHeapGetMaxFreeSize() );
	Lcd_Update();
	Lcd_Trans();
	++g_count;
}

/// 終了.
void pceAppExit( void )
{
}

void Title( void )
{
	unsigned long const pad = pcePadGet();
	if( pad & PAD_D )
	{
		pceAppReqExit( 0 );
	}
	if( pad & TRG_A )
	{
		ChangeMode( GameMode_Ready );
		g_dir = Direction_Up;
		g_score = 0;
	}
	pceFontSetType( 0 );
	pceFontSetPos( 32, 20 );
	pceFontPutStr( "ぐるぐるP/ECE" );
	pceFontSetPos( 24, 40 );
	pceFontPutStr( "10秒間十字キーを" );
	pceFontSetPos( 8, 50 );
	pceFontPutStr( "時計回りにぐるぐるしろ！" );
	if( g_count % 16 < 8 )
	{
		pceFontSetType( 2 );
		pceFontSetPos( 35, 66 );
		pceFontPutStr( "PUSH A TO START" );
	}
}

void Ready( void )
{
	if( g_count >= 2 * 30 )
	{
		ChangeMode( GameMode_Main );
	}
	pceFontSetType( 1 );
	pceFontSetPos( 44, 36 );
	pceFontPutStr( "READY" );
	Disp();
}

void Main( void )
{
	unsigned long const pad = pcePadGet();
	while( pad & s_pad[ g_dir ] )
	{
		if( ++g_dir >= Direction_Num )
		{
			g_dir -= Direction_Num;
		}
		++g_score;
	}
	if( g_count >= 300 )
	{
		ChangeMode( GameMode_Result );
	}
	Disp();
	pceFontSetType( 1 );
	pceFontSetPos( 44, 36 );
	pceFontPrintf( "%d.%3d", g_count / 30, ( g_count * 1000 / 30 ) % 1000 );
}

void Result( void )
{
	if( pcePadGet() & TRG_A )
	{
		ChangeMode( GameMode_Title );
	}
	pceFontSetType( 1 );
	pceFontSetPos( 50, 36 );
	pceFontPutStr( "END" );
	Disp();
}

