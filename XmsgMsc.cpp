/*
  Copyright 2019 www.dev5.cn, Inc. dev5@qq.com
 
  This file is part of X-MSG-IM.
 
  X-MSG-IM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  X-MSG-IM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with X-MSG-IM.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <libx-msg-im-xsc.h>
#include <libx-msg-msc-msg.h>
#include "XmsgMsc.h"

XmsgMsc* XmsgMsc::inst = new XmsgMsc();

XmsgMsc::XmsgMsc()
{

}

XmsgMsc* XmsgMsc::instance()
{
	return XmsgMsc::inst;
}

bool XmsgMsc::start(const char* path)
{
	Log::setInfo();
	shared_ptr<XmsgMscCfg> cfg = XmsgMscCfg::load(path); 
	if (cfg == nullptr)
		return false;
	Log::setLevel(cfg->cfgPb->log().level().c_str());
	Log::setOutput(cfg->cfgPb->log().output());
	Xsc::init();
	XmsgNeUsr::subEvnEstab([](shared_ptr<XmsgNeUsr> nu)
	{
		XmsgMscMgr::xmsgNeUsrEvnEstab(nu);
	});
	XmsgNeUsr::subEvnDisc([](shared_ptr<XmsgNeUsr> nu)
	{
		XmsgMscMgr::xmsgNeUsrEvnDisc(nu);
	});
	if (!XmsgDst::instance()->init())
		return false;
	shared_ptr<XscTcpServer> pubSever(new XscTcpServer(cfg->cfgPb->cgt(), shared_ptr<XmsgMscTcpLog>(new XmsgMscTcpLog()))); 
	if (!pubSever->startup(cfg->pubXscServerCfg()))
		return false;
	shared_ptr<XscTcpServer> priSever(new XscTcpServer(cfg->cfgPb->cgt(), shared_ptr<XmsgMscTcpLog>(new XmsgMscTcpLog()))); 
	if (!priSever->startup(cfg->priXscServerCfg()))
		return false;
	shared_ptr<XmsgImN2HMsgMgr> pubMsgMgr(new XmsgImN2HMsgMgr(pubSever));
	shared_ptr<XmsgImN2HMsgMgr> priMsgMgr(new XmsgImN2HMsgMgr(priSever));
	XmsgMscMsg::init(pubMsgMgr, priMsgMgr);
	if (!pubSever->publish() || !priSever->publish()) 
		return false;
	this->init4msc(pubSever); 
	Xsc::hold([](ullong now)
	{

	});
	return true;
}

bool XmsgMsc::init4msc(shared_ptr<XscTcpServer> tcpServer)
{
	list<shared_ptr<XmsgMscSuperior>> lis;
	for (int i = 0; i < XmsgMscCfg::instance()->cfgPb->superior_size(); ++i)
	{
		auto& msc = XmsgMscCfg::instance()->cfgPb->superior(i);
		SptrCgt cgt = ChannelGlobalTitle::parse(msc.cgt());
		if (cgt == nullptr)
		{
			LOG_FAULT("it`s a bug, please check config file, msc: %s", msc.ShortDebugString().c_str())
			return false;
		}
		shared_ptr<XmsgMscSuperiorNe> ne(new XmsgMscSuperiorNe(tcpServer, msc.addr(), msc.pwd(), msc.alg(), cgt));
		shared_ptr<XmsgMscSuperior> superior(new XmsgMscSuperior(ne));
		ne->msc = superior;
		ne->setXscUsr(superior);
		lis.push_back(superior);
		XmsgMscMgr::instance()->addSuperior(superior);
	}
	for (auto& it : lis)
		static_pointer_cast<XmsgMscSuperiorNe>(it->channel)->connect();
	unordered_map<string , shared_ptr<vector<string>>> map; 
	for (int i = 0; i < XmsgMscCfg::instance()->cfgPb->subordinate_size(); ++i)
	{
		auto& msc = XmsgMscCfg::instance()->cfgPb->subordinate(i);
		SptrCgt cgt = ChannelGlobalTitle::parse(msc.cgt());
		if (cgt == nullptr)
		{
			LOG_FAULT("it`s a bug, please check config file, msc: %s", msc.ShortDebugString().c_str())
			return false;
		}
		string node;
		if (!XmsgMscMgr::instance()->isSubordinate(cgt, node)) 
		{
			LOG_FAULT("it`s not a subordinate x-msg-msc, please check config file, msc: %s", msc.ShortDebugString().c_str())
			return false;
		}
		auto it = map.find(node);
		if (it == map.end())
		{
			shared_ptr<vector<string>> v(new vector<string>());
			v->push_back(cgt->toString());
			map[node] = v;
		} else
			it->second->push_back(cgt->toString());
	}
	for (auto& it : map)
		XmsgMscMgr::instance()->initSubordinate(it.first, it.second);
	return true;
}

XmsgMsc::~XmsgMsc()
{

}

