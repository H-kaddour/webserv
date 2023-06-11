//in lexer check if directive duplicate
//and in here when to save data check with std::count if parameter are repeated
//also i will check location /path when i enter it in map
//******NOW I WILL HANDLE IF WORD ARE REPETED FOR LISTEN AND LOCATION also check int value like listen 222
#include "../include/Parsing.hpp"
//#include "Location.hpp"

std::vector<std::pair<Parsing::t_tokens, std::string> >::iterator	Parsing::get_end_closing_braces(int)
{
	int	check;
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;

	for (; this->begin->first != OPEN_BRACES && this->begin != this->tokens.end(); this->begin++)
		;
	for (this->begin++, iter = this->begin, check = 1; check && iter != this->tokens.end(); iter++)
	{
		if (iter->first == OPEN_BRACES)
			check++;
		else if (iter->first == CLOSE_BRACES)
			check--;
	}
	return (iter - 1);
}

void	Parsing::check_wish_directive(int check, std::vector<std::string> data)
{
	typedef void(Default_serv::*func)(std::vector<std::string>);
	func serv_func[] = {&Default_serv::add_listen, &Default_serv::add_host, \
		&Default_serv::add_server_name, &Default_serv::add_status_page, \
			&Default_serv::add_return, &Default_serv::add_root, &Default_serv::add_index, \
			&Default_serv::add_allow_methods, &Default_serv::add_client_max_body_size, \
			&Default_serv::add_autoindex, &Default_serv::add_cgi_info, &Default_serv::add_upload};
	for (int	i = 0; i < static_cast<int>(this->directive_name.size()); i++)
	{
		if (!this->directive_itr->second.compare(this->directive_name[i].second))
		{
			if (!check)
				((*(this->servers.end() - 1)).*serv_func[i])(data);
			else
				((*(this->servers.end() - 1)).locations_getter().*serv_func[i])(data);
			return ;
		}
	}
}

void	Parsing::parse_directive(int check)
{
	std::vector<std::string>	data;
	this->directive_itr = this->begin;
	for (this->begin++; this->begin != this->tokens.end() && this->begin->first != SEMICOLON; this->begin++)
		data.push_back(this->begin->second);
	this->check_wish_directive(check, data);
}

void	Parsing::parse_location(void)
{
	std::vector<std::pair<t_tokens, std::string> >::iterator	iter;
	int	size_location = (*(this->servers.end() - 1)).get_location_size();
	//Location	location((*(this->servers.end() - 1)));
	Location	*location = new Location;
	std::pair<std::string, Location*> hold;
	//std::pair<std::string, Location*>	hold((this->begin + 1)->second, location);

	//std::vector<std::string> a;
	//a.push_back("222");
	//location.add_listen(a);
	//std::pair<std::string, Location> a;
	//a.first = (this->begin + 1)->second;
	//a.second = location;
	//(*(this->servers.end() - 1)).add_locations(a);
	
	//HENA MAFHMETCH 7TA 9LWA F BLAN DIAL DISTRUCTERS
	//(*(this->servers.end() - 1)).add_locations(std::make_pair((this->begin + 1)->second, location));
	hold.first = (this->begin + 1)->second;
	hold.second = location;
	(*(this->servers.end() - 1)).add_locations(hold);
	//while (1);
	if ((*(this->servers.end() - 1)).get_location_size() == size_location)
		throw ("Error: path of location is repeated.");
	iter = this->get_end_closing_braces(0);
	for (; this->begin != iter; this->begin++)
	{
		std::cout << "  " << this->begin->second << std::endl;
		if (this->begin->first == DIRECTIVE)
			this->parse_directive(1);
	}
}

void	Parsing::parse_server(void)
{
	Server	serv;

	this->servers.push_back(serv);
	//std::vector<std::string> a;
	//a.push_back("222");
	//this->servers[0].add_listen(a);
	this->end = this->get_end_closing_braces(0);
	for (; this->begin != this->end; this->begin++)
	{
		std::cout << this->begin->second << std::endl;
		if (this->begin->first == DIRECTIVE)
			this->parse_directive(0);
		else if (this->begin->first == LOCATION)
			this->parse_location();
		//return ;
	}
}

void	Parsing::save_data_in_the_server(void)
{
	for (this->begin = this->tokens.begin(); this->begin != this->tokens.end(); )
	{
		if (this->begin->first == SERVER)
		{
			this->parse_server();
			this->begin = this->end + 1;
		}
	}
}