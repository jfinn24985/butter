##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Wed Nov 8 20:00:39 2017
##END:date
##START:preamble
include $(ROOTDIR)/M_sys.mk

##END:preamble
##START:target bouml
BOUML_BOUMLSRC :=  \
	UmlOperation.cpp \
	UmlClassView.cpp \
	UmlSettingsCmd.cpp \
	UmlSettings.cpp \
	UmlBaseFormalParameter.cpp \
	UmlRelation.cpp \
	UmlDeploymentDiagram.cpp \
	ClassGlobalCmd.cpp \
	UmlUseCase.cpp \
	aVisibility.cpp \
	UmlBaseDeployment.cpp \
	UmlBaseComponentView.cpp \
	UmlUseCaseView.cpp \
	UmlClassDiagram.cpp \
	UmlBaseUseCase.cpp \
	UmlBaseAttribute.cpp \
	UmlNcRelation.cpp \
	UmlComponentDiagram.cpp \
	CppSettingsCmd.cpp \
	UmlFormalParameter.cpp \
	UmlBaseUseCaseDiagram.cpp \
	UmlBaseCollaborationDiagram.cpp \
	UmlBaseOperation.cpp \
	UmlAttribute.cpp \
	anItemKind.cpp \
	UmlBaseUseCaseView.cpp \
	UmlExtraClassMember.cpp \
	UmlBaseComponent.cpp \
	UmlBuiltin.cpp \
	UmlCollaborationDiagram.cpp \
	UmlActualParameter.cpp \
	UmlCom.cpp \
	UmlBaseDeploymentView.cpp \
	UmlBaseComponentDiagram.cpp \
	UmlBaseClassMember.cpp \
	UmlBaseClassDiagram.cpp \
	UmlTypeSpec.cpp \
	UmlNode.cpp \
	UmlDeploymentView.cpp \
	JavaSettingsCmd.cpp \
	UmlBaseItem.cpp \
	UmlPackage.cpp \
	UmlBaseNcRelation.cpp \
	UmlBaseDiagram.cpp \
	UmlBaseDeploymentDiagram.cpp \
	UmlBaseArtifact.cpp \
	UmlBaseClass.cpp \
	UmlSequenceDiagram.cpp \
	UmlClassMember.cpp \
	CmdFamily.cpp \
	UmlBaseExtraClassMember.cpp \
	UmlBaseActualParameter.cpp \
	UmlUseCaseDiagram.cpp \
	UmlStereotype.cpp \
	UmlComponentView.cpp \
	UmlBaseSequenceDiagram.cpp \
	UmlBaseRelation.cpp \
	UmlBaseClassView.cpp \
	UmlComponent.cpp \
	IdlSettingsCmd.cpp \
	PackageGlobalCmd.cpp \
	UmlClass.cpp \
	IdlSettings.cpp \
	CppSettings.cpp \
	UmlArtifact.cpp \
	OnInstanceCmd.cpp \
	MiscGlobalCmd.cpp \
	UmlClassItem.cpp \
	aDirection.cpp \
	UmlDiagram.cpp \
	aRelationKind.cpp \
	UmlBasePackage.cpp \
	UmlItem.cpp \
	UmlParameter.cpp \
	UmlBaseNode.cpp \
	JavaSettings.cpp \
	UmlBaseClassItem.cpp
BOUML_BOUML_DINC := $(filter %$(sufdep), $(BOUML_BOUMLSRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BOUML_BOUMLSRC:.c=$(sufdep)))
bouml$(suflib): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm
bouml$(suflib) $(BOUML_BOUML_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT 
BOUML_BOUML_OBJ+=$(filter %$(sufobj), $(BOUML_BOUMLSRC:.cpp=$(sufobj)))
USEROBJ+=$(BOUML_BOUML_OBJ)
bouml$(suflib) : $(BOUML_BOUML_SRC) $(BOUML_BOUML_DINC)
-include $(BOUML_BOUML_DINC)
$(eval $(call do_archive,bouml$(suflib),$(BOUML_BOUML_OBJ)))

all :: bouml$(suflib)
TARGETS+=bouml$(suflib)

$(eval $(call do_install,bouml$(suflib),LIB))

##END:target bouml
