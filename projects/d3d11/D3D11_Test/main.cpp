#include <iostream>
#include <SDL.h>
#include <SDL_syswm.h>
#include <d3d11.h>


struct Point2
{
	int x;
	int y;
};

template <typename T>
void safeRelease(T*& p_resource)
{
	if (p_resource != nullptr)
	{
		p_resource->Release();
		p_resource = nullptr;
	}
}


bool checkD3DResult(HRESULT p_result)
{
	if (FAILED(p_result))
	{
		return false;
	}
	return true;
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
	m_depthStencilBuffer(nullptr),
	m_depthStencilView(nullptr),
	m_debug(nullptr)
	{}

	~D3D11App() {}

	bool init(HWND p_windowHandle);
	void shutdown();

	void update();
	void render();

private:
	bool createDepthStencilBuffer();
	void clear();


	Point2                  m_backBufferSize;
	IDXGISwapChain*         m_swapChain;
	ID3D11Device*           m_d3dDevice;
	ID3D11DeviceContext*    m_d3dDeviceContext;
	ID3D11Texture2D*        m_swapChainBuffer;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D*        m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
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
	m_backBufferSize.x = swapChainDescription.BufferDesc.Width;
	m_backBufferSize.y = swapChainDescription.BufferDesc.Height;
	
	if (!checkD3DResult( D3D11CreateDeviceAndSwapChain(
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
		&m_d3dDeviceContext)))
	{
		return false;
	}

	if (!checkD3DResult(m_swapChain->GetBuffer(
		0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_swapChainBuffer))))
	{
		return false;
	}

	if (!checkD3DResult(m_d3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debug))))
	{
		return false;
	}

	if (!checkD3DResult(m_d3dDevice->CreateRenderTargetView(m_swapChainBuffer, 0, &m_renderTargetView)))
	{
		return false;
	}

	return true;
}


void D3D11App::shutdown()
{
	m_d3dDeviceContext->ClearState();

	safeRelease(m_debug);
	safeRelease(m_renderTargetView);
	safeRelease(m_swapChainBuffer);
	safeRelease(m_swapChain);
	safeRelease(m_depthStencilView);
	safeRelease(m_depthStencilBuffer);
	safeRelease(m_d3dDeviceContext);
	safeRelease(m_d3dDevice);
}


void D3D11App::update()
{
}


void D3D11App::render()
{
	ID3D11RenderTargetView* renderTargetViews[1] = { m_renderTargetView };
	ID3D11DepthStencilView* depthStencilView = m_depthStencilView;

	m_d3dDeviceContext->OMSetRenderTargets(1, renderTargetViews, depthStencilView);

	// Clear color & depth buffers
	clear();

	// Show current frame on screen
	checkD3DResult( m_swapChain->Present(0, 0) );
}


bool D3D11App::createDepthStencilBuffer()
{
	D3D11_TEXTURE2D_DESC textureDescription;
	textureDescription.Width = m_backBufferSize.x;
	textureDescription.Height = m_backBufferSize.y;
	textureDescription.MipLevels = 1;
	textureDescription.ArraySize = 1;
	textureDescription.Format = DXGI_FORMAT_D32_FLOAT;
	textureDescription.SampleDesc.Count = 1;
	textureDescription.SampleDesc.Quality = 0;
	textureDescription.Usage = D3D11_USAGE_DEFAULT;
	textureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDescription.CPUAccessFlags = 0;
	textureDescription.MiscFlags = 0;

	if (!checkD3DResult(m_d3dDevice->CreateTexture2D(&textureDescription, 0, &m_depthStencilBuffer)))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	// TODO: init desc

	if (!checkD3DResult(m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer, &desc, &m_depthStencilView)))
	{
		return false;
	}

	return true;
}


void D3D11App::clear()
{
	ID3D11RenderTargetView* renderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
	ID3D11DepthStencilView* depthStencilView = nullptr;

	m_d3dDeviceContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, renderTargetViews, &depthStencilView);

	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	{
		if (renderTargetViews[i] != nullptr)
		{
			float clearColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
			m_d3dDeviceContext->ClearRenderTargetView(renderTargetViews[i], clearColor);
			safeRelease(renderTargetViews[i]);
		}
	}

	if (depthStencilView != nullptr)
	{
		m_d3dDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	safeRelease(depthStencilView);
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

	SDL_Window* window = SDL_CreateWindow("Hello DirectX 11 World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
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

	// Main application loop
	bool keepRunning(true);
	while (keepRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)	keepRunning = false;
		}

		app.update();
		app.render();
	}

	app.shutdown();

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
