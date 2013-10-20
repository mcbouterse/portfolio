#include <iostream>
#include <SDL.h>
#include <SDL_syswm.h>
#include <d3d11.h>


template <typename T>
void safeRelease(T*& p_resource)
{
	if (p_resource != nullptr)
	{
		p_resource->Release();
		p_resource = nullptr;
	}
}


class D3D11App
{
public:
	D3D11App()
	:
	m_swapChain(nullptr),
	m_d3dDevice(nullptr),
	m_d3dDeviceContext(nullptr),
	m_swapChainBuffer(nullptr),
	m_renderTargetView(nullptr),
	m_debug(nullptr)
	{}

	~D3D11App() {}

	bool init(HWND p_windowHandle);
	void shutdown();

private:
	IDXGISwapChain*         m_swapChain;
	ID3D11Device*           m_d3dDevice;
	ID3D11DeviceContext*    m_d3dDeviceContext;
	ID3D11Texture2D*        m_swapChainBuffer;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Debug*            m_debug;
	D3D_FEATURE_LEVEL       m_featureLevel;
};


DXGI_SWAP_CHAIN_DESC createSwapChainDescription(HWND p_windowHandle)
{
	DXGI_SWAP_CHAIN_DESC result;

	DXGI_RATIONAL refreshRate = { 60, 1 };
	DXGI_MODE_DESC modeDescription;
	modeDescription.Width = 1024;
	modeDescription.Height = 768;
	modeDescription.RefreshRate = refreshRate;
	modeDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	modeDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	modeDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC sampleDescription;
	sampleDescription.Count = 1;
	sampleDescription.Quality = 0;

	result.BufferCount = 2;
	result.BufferDesc = modeDescription;
	result.SampleDesc = sampleDescription;
	result.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	result.OutputWindow = p_windowHandle;
	result.Windowed = TRUE;
	result.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	result.Flags = 0;

	return result;
}


bool D3D11App::init(HWND p_windowHandle)
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};

	DXGI_SWAP_CHAIN_DESC swapChainDescription = createSwapChainDescription(p_windowHandle);
	
	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr, // Default Adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, // No software device
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&swapChainDescription,
		&m_swapChain,
		&m_d3dDevice,
		&m_featureLevel,
		&m_d3dDeviceContext);

	if (FAILED(result))
	{
		std::cout << "Failed to intialize D3D11" << std::endl;
		return false;
	}

	result = m_swapChain->GetBuffer(
		0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_swapChainBuffer));

	if (FAILED(result))
	{
		return false;
	}

	result = m_d3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debug));

	result = m_d3dDevice->CreateRenderTargetView(m_swapChainBuffer, 0, &m_renderTargetView);

	return true;
}


void D3D11App::shutdown()
{
	safeRelease(m_debug);
	safeRelease(m_renderTargetView);
	safeRelease(m_swapChainBuffer);
	safeRelease(m_swapChain);
	safeRelease(m_d3dDeviceContext);
	safeRelease(m_d3dDevice);
}


int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	D3D11App app;

	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);
	if (SDL_GetWindowWMInfo(window, &windowInfo) == SDL_TRUE)
	{
		app.init(windowInfo.info.win.window);
	}

	SDL_Delay(2000);

	app.shutdown();

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
